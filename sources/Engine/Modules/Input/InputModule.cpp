#include "InputModule.h"

InputModule::InputModule()
{
}

InputModule::~InputModule()
{

}

void InputModule::registerAction(const std::string& actionName,
                                 const InputAction& action,
                                 const std::function<void (const InputActionArgs* const)>& handler)
{
    m_inputActions.push_back(action.clone());
    m_inputActionsState.insert({ actionName, InputActionState::Inactive });

    if (handler != nullptr) {
        m_inputActions[m_inputActions.size() - 1]->m_handler = handler;
    }
}

bool InputModule::isActionActive(const std::string &actionName) const
{
    return m_inputActionsState.at(actionName) == InputActionState::Active;
}

void InputModule::registerMouseMoveHandler(const std::function<void(const MoveMoveArgs * const)>& handler)
{
    m_mouseMoveHandlers.push_back(handler);
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
        toggleActionState(KeyboardInputAction(ev.key.keysym.sym), newState, &args);
    }
    else if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
        MouseButtonClickArgs args;
        args.button = ev.button.button;

        InputActionState newState = (ev.type == SDL_MOUSEBUTTONDOWN) ? InputActionState::Active : InputActionState::Inactive;
        toggleActionState(MouseButtonClickAction(ev.button.button), newState, &args);
    }
    else if (ev.type == SDL_MOUSEMOTION) {
        MoveMoveArgs args;
        args.position.x = ev.motion.x;
        args.position.y = ev.motion.y;

        for (auto& handler : m_mouseMoveHandlers) {
            handler(&args);
        }
    }
}

void InputModule::toggleActionState(const InputAction& action, InputActionState state, const InputActionArgs* const args)
{
    for (InputAction* currentAction : m_inputActions) {
        if (currentAction->equals(&action)) {
            if (currentAction->m_handler != nullptr) {
                if ((currentAction->m_handleMode == InputActionHandleMode::OnActivation && state == InputActionState::Active) ||
                        (currentAction->m_handleMode == InputActionHandleMode::OnDeactivation && state == InputActionState::Inactive) ||
                        currentAction->m_handleMode == InputActionHandleMode::Always)
                {
                    currentAction->m_handler(args);
                }
            }

            m_inputActionsState[currentAction->m_name] = state;
        }
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
