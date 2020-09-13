#include "precompiled.h"

#pragma hdrstop

#include "AudioSourceComponent.h"

#include <utility>

AudioSourceComponent::AudioSourceComponent(std::shared_ptr<AudioClip> clip)
  : m_source(std::make_shared<AudioSource>(std::move(clip)))
{

}

AudioSourceComponent::~AudioSourceComponent() = default;

const AudioSource& AudioSourceComponent::getSource() const
{
  return *m_source;
}

AudioSource& AudioSourceComponent::getSource()
{
  return *m_source;
}

AudioSource* AudioSourceComponent::getSourcePtr() const
{
  return m_source.get();
}
