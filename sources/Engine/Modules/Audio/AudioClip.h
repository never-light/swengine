#pragma once

#include <span>
#include <array>

#include <AL/al.h>
#include <AL/alc.h>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "types.h"

enum class AudioClipFormat {
  MONO_8,
  MONO_16,
  STEREO_8,
  STEREO_16
};

class AudioSource;

class AudioClip : public Resource {
 public:
  AudioClip(AudioClipFormat format, const std::byte* samples, size_t samplesSize, uint32_t frequency);
  ~AudioClip() override;

 private:
  [[nodiscard]] ALuint getALBuffer() const;

 private:
  ALuint m_buffer = 0;

 private:
  static constexpr std::array<ALenum, 4> AUDIO_CLIP_FORMAT_LOOKUP_TABLE = {
    AL_FORMAT_MONO8,
    AL_FORMAT_MONO16,
    AL_FORMAT_STEREO8,
    AL_FORMAT_STEREO16
  };

 private:
  friend class AudioSource;
};
