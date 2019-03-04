#include "Sound.h"
#include <iostream>

Sound::~Sound()
{
	alDeleteBuffers(1, &m_soundBufferID);
}

Sound::Sound(std::byte * soundData, size_t channels, size_t dataSize, unsigned int sampleRate)
{
	alGenBuffers(1, &m_soundBufferID);

	ALenum format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	alBufferData(m_soundBufferID, format, soundData, dataSize, sampleRate);
}

unsigned int  Sound::getSoundBufferID()
{
	return m_soundBufferID;
}