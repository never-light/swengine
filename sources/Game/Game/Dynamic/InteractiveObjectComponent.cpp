#include "InteractiveObjectComponent.h"

InteractiveObjectComponent::InteractiveObjectComponent()
{

}

void InteractiveObjectComponent::setTakeable(bool isTakeable)
{
  m_isTakeable = isTakeable;
}

bool InteractiveObjectComponent::isTakeable() const
{
  return m_isTakeable;
}

void InteractiveObjectComponent::setUsable(bool isUsable)
{
  m_isUsable = isUsable;
}

bool InteractiveObjectComponent::isUsable() const
{
  return m_isUsable;
}

void InteractiveObjectComponent::setUseCallback(const InteractiveObjectComponent::ActionCallback& callback)
{
  m_useCallback = callback;
}

InteractiveObjectComponent::ActionCallback InteractiveObjectComponent::getUseCallback() const
{
  return m_useCallback;
}

void InteractiveObjectComponent::setTakeCallback(const InteractiveObjectComponent::ActionCallback& callback)
{
  m_takeCallback = callback;
}

InteractiveObjectComponent::ActionCallback InteractiveObjectComponent::getTakeCallback() const
{
  return m_takeCallback;
}

void InteractiveObjectComponent::setName(const std::string& name)
{
  m_name = name;
}

const std::string& InteractiveObjectComponent::getName() const
{
  return m_name;
}

void InteractiveObjectComponent::setTalkable(bool isTalkable)
{
  m_isTalkable = isTalkable;
}

bool InteractiveObjectComponent::isTalkable() const
{
  return m_isTalkable;
}

InteractiveObjectComponent::BindingParameters InteractiveObjectComponent::getBindingParameters() const
{
  return InteractiveObjectComponent::BindingParameters{
    .title = m_name,
    .isTakeable = m_isTakeable,
    .isTalkable = m_isTalkable,
    .isUsable = m_isUsable
  };
}

InteractiveComponentBinder::InteractiveComponentBinder(const ComponentBindingParameters& componentParameters)
  : m_bindingParameters(componentParameters)
{

}

void InteractiveComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& interactiveComponent = *gameObject.addComponent<InteractiveObjectComponent>().get();

  interactiveComponent.setName(m_bindingParameters.title);
  interactiveComponent.setTakeable(m_bindingParameters.isTakeable);
  interactiveComponent.setUsable(m_bindingParameters.isUsable);
  interactiveComponent.setTalkable(m_bindingParameters.isTalkable);
}
