#include "SoundSystem.h"

SoundSystem::SoundSystem()
	: m_currentSoundListener(nullptr)
{
	m_alDevice = alcOpenDevice(NULL);
	m_alContext = alcCreateContext(m_alDevice, NULL);
	alcMakeContextCurrent(m_alContext);

	m_currentSoundListener = new SoundListener();
}

SoundSystem::~SoundSystem()
{
	alcDestroyContext(m_alContext);
	alcCloseDevice(m_alDevice);
}

void SoundSystem::update(float delta)
{
}

SoundListener * SoundSystem::getCurrentSoundListener() const
{
	return m_currentSoundListener;
}
