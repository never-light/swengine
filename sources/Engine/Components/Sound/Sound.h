#pragma once

#include <cstddef>
#include <OpenAL/al.h>

class Sound {
public:
	Sound(std::byte* soundData, size_t  channels,size_t  dataSize, unsigned int sampleRate);
	~Sound();
	    
	unsigned int getSoundBufferID();

private:
	unsigned int m_soundBufferID;
};