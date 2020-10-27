#include "precompiled.h"

#pragma hdrstop

#include "AudioClipResourceManager.h"

#include "Utility/helpers.h"

DISABLE_WARNINGS()
#include <stb_vorbis.c>
ENABLE_WARNINGS()

#include "Exceptions/exceptions.h"

AudioClipResourceManager::AudioClipResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<AudioClip, AudioClipResourceConfig>(resourcesManager)
{

}

AudioClipResourceManager::~AudioClipResourceManager() = default;

void AudioClipResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  AudioClipResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Audio clip resource refer to not existing file", resourceConfig->resourcePath));
  }
}


void AudioClipResourceManager::load(size_t resourceIndex)
{
  AudioClipResourceConfig* config = getResourceConfig(resourceIndex);

  std::byte* audioData;
  int channelsCount = 0;
  int sampleRate = 0;

  // NOTE: dataLength is number of samples in the audio file
  int dataLength = stb_vorbis_decode_filename(config->resourcePath.c_str(),
    &channelsCount,
    &sampleRate,
    reinterpret_cast<short**>(&audioData));

  if (dataLength <= 0) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Trying to load invalid sound file {}", config->resourcePath));
  }

  AudioClipFormat format;

  if (channelsCount == 1) {
    format = AudioClipFormat::MONO_16;
    dataLength *= 2;
  }
  else if (channelsCount == 2) {
    format = AudioClipFormat::STEREO_16;
    dataLength *= 2 * 2;
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Audio file has invalid number of audio channels");
  }

  allocateResource<AudioClip>(resourceIndex, format, audioData, dataLength, sampleRate);

  free(audioData);
}
