#include "precompiled.h"

#pragma hdrstop

#include "AudioSource.h"

#include <utility>

#include "ALDebug.h"

// TODO: do not generate AL sources in constructors. Create and use sources pool instead.

AudioSource::AudioSource(std::shared_ptr<AudioClip> clip)
{
  AL_CALL(alGenSources(1, &m_source));

  AL_CALL_BLOCK_BEGIN();
  setClip(std::move(clip));

  setPitch(1.0f);
  setVolume(1.0f);
  setPosition({0.0f, 0.0f, 0.0f});
  setVelocity({0.0, 0.0f, 0.0f});
  setLooped(false);

  SW_ASSERT(alIsSource(m_source));
  AL_CALL_BLOCK_END();
}


AudioSource::AudioSource(const AudioSource& source)
{
  AL_CALL(alGenSources(1, &m_source));

  AL_CALL_BLOCK_BEGIN();
  setClip(std::move(source.getClip()));

  setPitch(source.getPitch());
  setVolume(source.getVolume());
  setPosition(source.getPosition());
  setVelocity(source.getVelocity());
  setLooped(source.isLooped());
  setRelativeToListenerMode(source.isRelativeToListener());

  SW_ASSERT(alIsSource(m_source));
  AL_CALL_BLOCK_END();

}

AudioSource::~AudioSource()
{
  if (m_source != 0) {
    alDeleteSources(1, &m_source);
  }
}

ALuint AudioSource::getALSource() const
{
  return m_source;
}

void AudioSource::setPitch(float pitch)
{
  alSourcef(m_source, AL_PITCH, pitch);
}

float AudioSource::getPitch() const
{
  float pitch;
  alGetSourcef(m_source, AL_PITCH, &pitch);

  return pitch;
}

void AudioSource::setVolume(float volume)
{
  alSourcef(m_source, AL_GAIN, volume);
}

float AudioSource::getVolume() const
{
  float volume;
  alGetSourcef(m_source, AL_GAIN, &volume);

  return volume;
}

void AudioSource::setClip(std::shared_ptr<AudioClip> clip)
{
  m_audioClip = std::move(clip);

  alSourcei(m_source, AL_BUFFER, m_audioClip->getALBuffer());
}

std::shared_ptr<AudioClip> AudioSource::getClip() const
{
  return m_audioClip;
}

void AudioSource::setPosition(const glm::vec3& position)
{
  alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 AudioSource::getPosition() const
{
  glm::vec3 position;
  alGetSource3f(m_source, AL_POSITION, &position.x, &position.y, &position.z);

  return position;
}

void AudioSource::setVelocity(const glm::vec3& velocity)
{
  alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

glm::vec3 AudioSource::getVelocity() const
{
  glm::vec3 velocity;
  alGetSource3f(m_source, AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);

  return velocity;
}

void AudioSource::play()
{
  AL_CALL(alSourcePlay(m_source));
  m_sourceState = AudioSourceState::Playing;
}

bool AudioSource::isPlaying() const
{
  return m_sourceState == AudioSourceState::Playing;
}

void AudioSource::pause()
{
  AL_CALL(alSourcePause(m_source));
  m_sourceState = AudioSourceState::Paused;
}

bool AudioSource::isPaused() const
{
  return m_sourceState == AudioSourceState::Paused;
}

void AudioSource::stop()
{
  AL_CALL(alSourceStop(m_source));
  m_sourceState = AudioSourceState::Stopped;
}

bool AudioSource::isStopped() const
{
  return m_sourceState == AudioSourceState::Stopped;
}

void AudioSource::updateInternalState()
{
  ALint sourceState{};
  alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);

  switch (sourceState) {
    case AL_PLAYING:
      m_sourceState = AudioSourceState::Playing;
      break;
    case AL_PAUSED:
      m_sourceState = AudioSourceState::Paused;
      break;
    case AL_INITIAL:
    case AL_STOPPED:
      m_sourceState = AudioSourceState::Stopped;
      break;
    default:
      SW_ASSERT(false);
      break;
  }

  auto sourcesIt = m_subSources.begin();

  while (sourcesIt != m_subSources.end())
  {
    sourcesIt->updateInternalState();

    if (sourcesIt->isStopped())
    {
      sourcesIt = m_subSources.erase(sourcesIt);
    }
    else
    {
      ++sourcesIt;
    }
  }
}

void AudioSource::setLooped(bool isLooped)
{
  alSourcei(m_source, AL_LOOPING, isLooped);
}

bool AudioSource::isLooped() const
{
  ALint isLooped{};
  alGetSourcei(m_source, AL_LOOPING, &isLooped);

  return static_cast<bool>(isLooped);
}

void AudioSource::setRelativeToListenerMode(bool relativeToListener)
{
  alSourcei(m_source, AL_SOURCE_RELATIVE, relativeToListener);
}

bool AudioSource::isRelativeToListener() const
{
  ALint isRelativeToListener{};
  alGetSourcei(m_source, AL_SOURCE_RELATIVE, &isRelativeToListener);

  return static_cast<bool>(isRelativeToListener);
}

void AudioSource::playOnce(std::shared_ptr<AudioClip> clip)
{
  m_subSources.emplace_back(*this);
  m_subSources.rbegin()->setLooped(false);
  m_subSources.rbegin()->setClip(std::move(clip));

  m_subSources.rbegin()->play();
}
