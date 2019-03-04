#include <iostream>

#include "SoundSource.h"

SoundSource::SoundSource()
{
}

SoundSource::SoundSource(const vector3 & position, const vector3 & velocity, bool relative, float gain, float pitch, bool looped ) : m_position(position), m_velocity(velocity), m_relative(relative),
	m_gain(gain), m_pitch(pitch), m_looped(looped)
{
	alGenSources(1, &this->sourceID);
	alSourcef(this->sourceID, AL_PITCH, pitch);
	alSourcef(this->sourceID, AL_GAIN, gain);
	alSource3f(this->sourceID, AL_POSITION, position.x, position.y, position.z);
	alSource3f(this->sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	alSourcei(this->sourceID, AL_SOURCE_RELATIVE, relative);
	alSourcei(this->sourceID, AL_LOOPING, looped);
};

SoundSource::~SoundSource()
{
	//alDeleteSources(1, &this->sourceID);
}

void SoundSource::bindSound(Sound* sound)
{
	this->sound = sound;
	alSourcei(this->sourceID, AL_BUFFER, sound->getSoundBufferID());
	std::cout << "SourseBinding - " << alGetError() << std::endl;
}

void SoundSource::play()
{
	alSourcePlay(this->sourceID);
}

void SoundSource::pause()
{
	alSourcePause(this->sourceID);
}

void SoundSource::pauseOn(ALuint pauseTime)
{
	if (!this->isPlaying()) return;

	this->pause();
	Sleep(pauseTime);
	this->play();

}

void SoundSource::stop()
{
	alSourceStop(this->sourceID);
}

void SoundSource::setSpeed(ALfloat speed)
{
	setPitch(speed);
}

bool SoundSource::isPaused()
{
	ALint value;
	alGetSourcei(this->sourceID, AL_SOURCE_STATE, &value);
	return value == AL_PAUSED;
}

bool SoundSource::isPlaying()
{
	ALint value;
	alGetSourcei(this->sourceID, AL_SOURCE_STATE, &value);
	return value == AL_PLAYING;
}

unsigned int  SoundSource::getSourceID()
{
	return this->sourceID;
}

void SoundSource::setPosition(const vector3 & position)
{
	this ->m_position = position;
	alSource3f(this->sourceID, AL_POSITION, position.x, position.y, position.z);
}

void SoundSource::setVelocity(const vector3 & velocity)
{
	this->m_velocity = velocity;
	alSource3f(this->sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void SoundSource::setRelative(bool relative)
{
	this->m_relative = relative;
	alSourcei(this->sourceID, AL_SOURCE_RELATIVE, relative);
}

void SoundSource::setGain(float gain)
{
	this->m_gain = gain;
	alSourcef(this->sourceID, AL_GAIN, gain);
}

void SoundSource::setPitch(float pitch)
{
	this->m_pitch = pitch;
	alSourcef(this->sourceID, AL_PITCH, pitch);
}

void SoundSource::isLooped(bool looped)
{
	this->m_looped = looped;
	alSourcei(this->sourceID, AL_LOOPING, looped);
}

 vector3 & SoundSource::getPosition()
{
	return m_position;
}

 vector3 & SoundSource::getVelocity()
{
	return this->m_velocity;
}

float SoundSource::getGain()
{
	return this->m_gain;
}

float SoundSource::getPitch()
{
	return this->m_pitch;
}

bool SoundSource::getLooped()
{
	return this->m_looped;
}
