#include "precompiled.h"

#pragma hdrstop

#include "InputSystem.h"

#include <utility>
#include "InputEvents.h"

InputSystem::InputSystem(std::shared_ptr<InputModule> inputModule)
  : m_inputModule(std::move(inputModule))
{

}

void InputSystem::configure()
{
  m_inputModule->registerEventsListener(shared_from_this());
}

void InputSystem::unconfigure()
{
  m_inputModule->unregisterEventsListener(shared_from_this());
}

void InputSystem::processInputActionToggleEvent(const InputActionToggleEvent& event)
{
  getGameWorld()->emitEvent(event);
}

void InputSystem::processMouseMoveEvent(const MouseMoveEvent& event)
{
  getGameWorld()->emitEvent(event);
}

void InputSystem::processKeyboardEvent(const KeyboardEvent& event)
{
  getGameWorld()->emitEvent(event);
}

void InputSystem::processMouseButtonEvent(const MouseButtonEvent& event)
{
  getGameWorld()->emitEvent(event);
}

void InputSystem::processMouseWheelEvent(const MouseWheelEvent& event)
{
  getGameWorld()->emitEvent(event);
}
