#include "precompiled.h"

#pragma hdrstop

#include "AudioSourceComponent.h"

#include <utility>
#include "Modules/ECS/ECS.h"

AudioSourceComponent::AudioSourceComponent(ResourceHandle<AudioClip> clip)
  : m_source(std::make_shared<AudioSource>(clip))
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

AudioSourceComponent::BindingParameters AudioSourceComponent::getBindingParameters() const
{
  return AudioSourceComponent::BindingParameters{
    .clipResourceName = m_source->getClip().getResourceId(),
    .cameraRelative = m_source->isRelativeToListener(),
    .isLooped = m_source->isLooped(),
    .pitch = m_source->getPitch(),
    .volume = m_source->getVolume(),
    .position = m_source->getPosition(),
  };
}


AudioSourceComponentBinder::AudioSourceComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void AudioSourceComponentBinder::bindToObject(GameObject& gameObject)
{
  ResourceHandle<AudioClip> audioClipInstance =
    m_resourcesManager->getResource<AudioClip>(m_bindingParameters.clipResourceName);

  auto& audioSourceComponent = *gameObject.addComponent<AudioSourceComponent>(audioClipInstance).get();
  auto& audioSource = audioSourceComponent.getSource();

  audioSource.setVolume(m_bindingParameters.volume);
  audioSource.setPitch(m_bindingParameters.pitch);
  audioSource.setRelativeToListenerMode(m_bindingParameters.cameraRelative);
  audioSource.setLooped(m_bindingParameters.isLooped);

  audioSource.setPosition(m_bindingParameters.position);
}

