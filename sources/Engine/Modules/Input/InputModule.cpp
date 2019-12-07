#include "InputModule.h"

InputModule::InputModule(SDL_Window* window)
    : m_window(window)
{

}

InputModule::~InputModule()
{
    for (InputAction* action : m_inputActions)
        delete action;
}

void InputModule::registerAction(const std::string& actionName,
                                 const InputAction& action)
{
    InputAction* actionClone = action.clone();
    actionClone->m_name = actionName;

    m_inputActions.push_back(actionClone);

    m_inputActionsState.insert({ actionName, InputActionState::Inactive });
}

bool InputModule::isActionActive(const std::string &actionName) const
{
    return m_inputActionsState.at(actionName) == InputActionState::Active;
}

void InputModule::setMouseMovementMode(MouseMovementMode mode)
{
    if (mode == MouseMovementMode::Relative) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else if (mode == MouseMovementMode::Absolute) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

MouseMovementMode InputModule::getMouseMovementMode() const
{
    SDL_bool relativeMode = SDL_GetRelativeMouseMode();

    return (relativeMode == SDL_TRUE) ? MouseMovementMode::Relative : MouseMovementMode::Absolute;
}

void InputModule::setMousePosition(const MousePosition& position)
{
    SDL_WarpMouseInWindow(m_window, position.x, position.y);
}

MousePosition InputModule::getMousePosition() const
{
    MousePosition position;
    SDL_GetMouseState(&position.x, &position.y);

    return position;
}

void InputModule::processRawSDLEvent(const SDL_Event &ev)
{
    if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
        KeyboardInputArgs args;
        args.keyCode = ev.key.keysym.sym;
        args.repeated = ev.key.repeat;

        InputActionState newState = (ev.type == SDL_KEYDOWN) ? InputActionState::Active : InputActionState::Inactive;
        toggleActionState(KeyboardInputAction(ev.key.keysym.sym), newState, args);
    }
    else if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
        MouseButtonClickArgs args;
        args.button = ev.button.button;

        InputActionState newState = (ev.type == SDL_MOUSEBUTTONDOWN) ? InputActionState::Active : InputActionState::Inactive;
        toggleActionState(MouseButtonClickAction(ev.button.button), newState, args);
    }
    else if (ev.type == SDL_MOUSEMOTION) {
        MouseMoveEvent event;
        event.x = ev.motion.x;
        event.y = ev.motion.y;

        event.deltaX = ev.motion.xrel;
        event.deltaY = ev.motion.yrel;

        triggerInputEvent(InputEvent{ event });
    }
}

void InputModule::registerEventsListener(std::shared_ptr<InputEventsListener> listener)
{
    m_eventsListeners.push_back(listener);
}

void InputModule::unregisterEventsListener(std::shared_ptr<InputEventsListener> listener)
{
    m_eventsListeners.erase(std::remove(m_eventsListeners.begin(), m_eventsListeners.end(), listener),
                            m_eventsListeners.end());
}

void InputModule::toggleActionState(const InputAction& action, InputActionState state, const InputActionToggleEventArgs& args)
{
    for (InputAction* currentAction : m_inputActions) {
        if (currentAction->equals(&action)) {
            m_inputActionsState[currentAction->m_name] = state;

            InputActionToggleEvent event;
            event.actionName = action.m_name;
            event.newState = state;
            event.args = args;

            triggerInputEvent(InputEvent{ event });
        }
    }
}

void InputModule::triggerInputEvent(const InputEvent& event)
{
    for (auto eventsListener : m_eventsListeners) {
        eventsListener->processInputEvent(event);
    }
}

InputAction::InputAction()
{

}

InputAction::~InputAction()
{

}

KeyboardInputAction::KeyboardInputAction(SDL_Keycode keyCode)
    : m_keyCode(keyCode)
{

}

SDL_Keycode KeyboardInputAction::getKeyCode() const
{
    return m_keyCode;
}

InputAction *KeyboardInputAction::clone() const {
    return new KeyboardInputAction(m_keyCode);
}

bool KeyboardInputAction::equals(const InputAction * const action) const {
    const KeyboardInputAction* const keyboardAction = dynamic_cast<const KeyboardInputAction* const>(action);
    return keyboardAction != nullptr && keyboardAction->getKeyCode() == m_keyCode;
}

InputActionArgs::InputActionArgs()
{

}

InputActionArgs::~InputActionArgs()
{

}

MouseButtonClickAction::MouseButtonClickAction(uint8_t button)
    : m_button(button)
{

}

uint8_t MouseButtonClickAction::getButtonCode() const
{
    return m_button;
}

InputAction* MouseButtonClickAction::clone() const
{
    return new MouseButtonClickAction(m_button);
}

bool MouseButtonClickAction::equals(const InputAction * const action) const
{
    const MouseButtonClickAction* const clickAction = dynamic_cast<const MouseButtonClickAction* const>(action);
    return clickAction != nullptr && clickAction->getButtonCode() == m_button;
}
