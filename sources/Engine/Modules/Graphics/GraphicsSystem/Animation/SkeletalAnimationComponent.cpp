#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationComponent.h"

#include <utility>

#include "Modules/ECS/ECS.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(
  ResourceHandle<AnimationStatesMachine> animationStatesMachine)
  : m_animationStatesMachine(std::move(animationStatesMachine))
{

}

AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachineRef()
{
  return *m_animationStatesMachine;
}

const AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachineRef() const
{
  return *m_animationStatesMachine;
}

const AnimationMatrixPalette& SkeletalAnimationComponent::getMatrixPalette() const
{
  return m_animationStatesMachine->getCurrentMatrixPalette();
}

void SkeletalAnimationComponent::setAnimationStatesMachine(ResourceHandle<AnimationStatesMachine> statesMachine)
{
  m_animationStatesMachine = std::move(statesMachine);
}

ResourceHandle<AnimationStatesMachine> SkeletalAnimationComponent::getAnimationStatesMachine() const
{
  return m_animationStatesMachine;
}

SkeletalAnimationComponent::BindingParameters SkeletalAnimationComponent::getBindingParameters() const
{
  return SkeletalAnimationComponent::BindingParameters{
    .skeletonResourceName = m_animationStatesMachine->getSkeleton().getResourceId(),
    .stateMachineResourceName = m_animationStatesMachine.getResourceId(),
    .stateMachineInitialState = m_animationStatesMachine->getActiveState().getName(),
  };
}


AnimationComponentBinder::AnimationComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void AnimationComponentBinder::bindToObject(GameObject& gameObject)
{
  auto skeletonInstance = m_resourcesManager->getResource<Skeleton>(m_bindingParameters.skeletonResourceName);
  auto statesMachineInstance = m_resourcesManager->getResource<AnimationStatesMachine>(
    m_bindingParameters.stateMachineResourceName);

  gameObject.addComponent<SkeletalAnimationComponent>(statesMachineInstance);

  const std::string& initialStateMachineStateName = m_bindingParameters.stateMachineInitialState;

  if (!initialStateMachineStateName.empty()) {
    statesMachineInstance->setActiveState(initialStateMachineStateName);
  }
}
