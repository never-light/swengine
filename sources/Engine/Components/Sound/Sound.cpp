#include "Sound.h"
#include <iostream>

Sound::~Sound()
{
	//alDeleteBuffers(1,&this->soundBufferID);
}

Sound::Sound(std::byte * soundData, size_t channels, size_t dataSize, unsigned int sampleRate)
{
	alGenBuffers(1, &this->soundBufferID);

	ALenum format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	alBufferData(this->soundBufferID, format, soundData, dataSize, sampleRate);
	std::cout << "BufferData - " << alGetError() << std::endl;

}

unsigned int  Sound::getSoundBufferID()
{
	return this->soundBufferID;
}
