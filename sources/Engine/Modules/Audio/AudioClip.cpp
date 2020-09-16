#include "precompiled.h"

#pragma hdrstop

#include "AudioClip.h"
#include "ALDebug.h"

AudioClip::AudioClip(AudioClipFormat format, const std::byte* samples, size_t samplesSize, uint32_t frequency)
{
  AL_CALL(alGenBuffers(1, &m_buffer));

  AL_CALL(alBufferData(m_buffer,
    AUDIO_CLIP_FORMAT_LOOKUP_TABLE[static_cast<size_t>(format)],
    samples,
    static_cast<ALsizei>(samplesSize),
    static_cast<ALsizei>(frequency)));
}

AudioClip::~AudioClip()
{
  AL_CALL(alDeleteBuffers(1, &m_buffer));
}

ALuint AudioClip::getALBuffer() const
{
  return m_buffer;
}
