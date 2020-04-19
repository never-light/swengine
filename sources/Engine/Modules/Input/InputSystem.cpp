#include "precompiled.h"

#pragma hdrstop

#include "InputSystem.h"
#include "InputEvents.h"

InputSystem::InputSystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<InputModule> inputModule)
  : m_gameWorld(gameWorld),
    m_inputModule(inputModule)
{

}

void InputSystem::configure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  m_inputModule->registerEventsListener(shared_from_this());
}

void InputSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  m_inputModule->unregisterEventsListener(shared_from_this());
}

void InputSystem::processInputActionToggleEvent(const InputActionToggleEvent& event)
{
  m_gameWorld->emitEvent(event);
}

void InputSystem::processMouseMoveEvent(const MouseMoveEvent& event)
{
  m_gameWorld->emitEvent(event);
}

void InputSystem::processKeyboardEvent(const KeyboardEvent& event)
{
  m_gameWorld->emitEvent(event);
}

void InputSystem::processMouseButtonEvent(const MouseButtonEvent& event)
{
  m_gameWorld->emitEvent(event);
}

void InputSystem::processMouseWheelEvent(const MouseWheelEvent& event)
{
  m_gameWorld->emitEvent(event);
}
