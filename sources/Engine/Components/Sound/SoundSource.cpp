#include <iostream>

#include "SoundSource.h"

SoundSource::SoundSource()
	: m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_relative(false),
	m_gain(1.0f),
	m_pitch(1.0f),
	m_looped(false)
{
	initializeALSoundSource();
}

SoundSource::SoundSource(const vector3 & position, const vector3 & velocity, 
	bool relative, float gain, float pitch, bool looped )
	: m_position(position),
	m_velocity(velocity), 
	m_relative(relative),
	m_gain(gain), 
	m_pitch(pitch), 
	m_looped(looped)
{
	initializeALSoundSource();
}
SoundSource::SoundSource(SoundSource && source)
	: m_position(source.m_position),
	m_velocity(source.m_velocity),
	m_looped(source.m_looped),
	m_pitch(source.m_pitch),
	m_gain(source.m_gain),
	m_relative(source.m_relative),
	m_sound(source.m_sound),
	m_sourceID(source.m_sourceID)
{
	source.m_sourceID = 0;
}

SoundSource::~SoundSource()
{
	if (m_sourceID != 0)
		alDeleteSources(1, &m_sourceID);
}

void SoundSource::bindSound(Sound* sound)
{
	m_sound = sound;
	alSourcei(m_sourceID, AL_BUFFER, sound->getSoundBufferID());
}

void SoundSource::play()
{
	alSourcePlay(m_sourceID);
}

void SoundSource::pause()
{
	alSourcePause(m_sourceID);
}

void SoundSource::pauseOn(ALuint pauseTime)
{
	if (!this->isPlaying()) return;

	// TODO: Implement pause for a while

	/*this->pause();
	Sleep(pauseTime);
	this->play();*/
}

void SoundSource::stop()
{ 
	alSourceStop(m_sourceID);
}

void SoundSource::setSpeed(ALfloat speed)
{
	setPitch(speed);
}

void SoundSource::initializeALSoundSource()
{
	alGenSources(1, &m_sourceID);
	alSourcef(m_sourceID, AL_PITCH, m_pitch);
	alSourcef(m_sourceID, AL_GAIN, m_gain);
	alSource3f(m_sourceID, AL_POSITION, m_position.x, m_position.y, m_position.z);
	alSource3f(m_sourceID, AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
	alSourcei(m_sourceID, AL_SOURCE_RELATIVE, m_relative);
	alSourcei(m_sourceID, AL_LOOPING, m_looped);
}

bool SoundSource::isPaused() const
{
	ALint value;
	alGetSourcei(m_sourceID, AL_SOURCE_STATE, &value);
	return value == AL_PAUSED;
}

bool SoundSource::isPlaying() const
{
	ALint value;
	alGetSourcei(m_sourceID, AL_SOURCE_STATE, &value);
	return value == AL_PLAYING;
}

unsigned int  SoundSource::getSourceID() const
{
	return m_sourceID;
}

void SoundSource::setPosition(const vector3 & position)
{
	this ->m_position = position;
	alSource3f(m_sourceID, AL_POSITION, position.x, position.y, position.z);
}

void SoundSource::setVelocity(const vector3 & velocity)
{
	this->m_velocity = velocity;
	alSource3f(m_sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void SoundSource::setRelative(bool relative)
{
	this->m_relative = relative;
	alSourcei(m_sourceID, AL_SOURCE_RELATIVE, relative);
}

void SoundSource::setGain(float gain)
{
	this->m_gain = gain;
	alSourcef(m_sourceID, AL_GAIN, gain);
}

void SoundSource::setPitch(float pitch)
{
	this->m_pitch = pitch;
	alSourcef(m_sourceID, AL_PITCH, pitch);
}

void SoundSource::setLooped(bool looped)
{
	this->m_looped = looped;
	alSourcei(m_sourceID, AL_LOOPING, looped);
}

vector3 SoundSource::getPosition() const
{
	return m_position;
}

vector3 SoundSource::getVelocity() const
{
	return m_velocity;
}

float SoundSource::getGain() const
{
	return m_gain;
}

float SoundSource::getPitch() const
{
	return m_pitch;
}

bool SoundSource::isLooped() const
{
	return m_looped;
}
