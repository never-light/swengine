#include "InputSystem.h"

InputSystem::InputSystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<InputModule> inputModule)
    : m_gameWorld(gameWorld),
      m_inputModule(inputModule)
{

}

void InputSystem::processInputEvent(const InputEvent& event)
{
    m_gameWorld->emitEvent<InputEvent>(event);
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
