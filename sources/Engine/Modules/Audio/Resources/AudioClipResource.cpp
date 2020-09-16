#include "precompiled.h"

#pragma hdrstop

#include "AudioClipResource.h"

#include "Utility/helpers.h"

DISABLE_WARNINGS()
#include <stb_vorbis.c>
ENABLE_WARNINGS()

#include "Exceptions/exceptions.h"

AudioClipResource::AudioClipResource() = default;

AudioClipResource::~AudioClipResource()
{
  SW_ASSERT(m_audioClip.use_count() <= 1);
}

void AudioClipResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
  ARG_UNUSED(resourceManager);

  SW_ASSERT(m_audioClip == nullptr);

  auto parameters = declaration.getParameters<ParametersType>();

  if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
    m_audioClip = loadFromFile(sourceFile->path, parameters);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load audio clip resource from invalid source");
  }
}

void AudioClipResource::unload()
{
  SW_ASSERT(m_audioClip.use_count() == 1);

  m_audioClip.reset();
}

bool AudioClipResource::isBusy() const
{
  return m_audioClip.use_count() > 1;
}

std::shared_ptr<AudioClip> AudioClipResource::loadFromFile(const std::string& path,
  const ParametersType& parameters)
{
  ARG_UNUSED(parameters);

  std::byte *audioData;
  int channelsCount = 0;
  int sampleRate = 0;

  // NOTE: dataLength is number of samples in the audio file
  int dataLength = stb_vorbis_decode_filename(path.c_str(),
    &channelsCount,
    &sampleRate,
    reinterpret_cast<short**>(&audioData));

  if (dataLength <= 0) {
    THROW_EXCEPTION(EngineRuntimeException, std::string("Trying to load invalid sound file ") + path);
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

  auto audioClip = std::make_shared<AudioClip>(format, audioData, dataLength, sampleRate);

  free(audioData);

  return audioClip;
}

AudioClipResource::ParametersType AudioClipResource::buildDeclarationParameters(
  const pugi::xml_node& declarationNode,
  const ParametersType& defaultParameters)
{
  ARG_UNUSED(declarationNode);

  ParametersType parameters = defaultParameters;

  return parameters;
}

std::shared_ptr<AudioClip> AudioClipResource::getAudioClip() const
{
  return m_audioClip;
}
