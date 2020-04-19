#include "precompiled.h"

#pragma hdrstop

#include "InputModule.h"

#include <spdlog/spdlog.h>

InputModule::InputModule(SDL_Window* window)
  : m_window(window)
{

}

InputModule::~InputModule()
{
  for (InputAction* action : m_inputActions) {
    delete action;
  }
}

void InputModule::registerAction(const std::string& actionName,
  const InputAction& action)
{
  InputAction* actionClone = action.clone();
  actionClone->m_name = actionName;

  m_inputActions.push_back(actionClone);

  m_inputActionsState.insert({actionName, InputActionState::Inactive});
}

void InputModule::unregisterAction(const std::string& actionName)
{
  auto inputActionStateIt = m_inputActionsState.find(actionName);

  if (inputActionStateIt == m_inputActionsState.end()) {
    spdlog::warn("Input: Failed to remove nonexistent action");

    return;
  }

  m_inputActions.erase(std::remove_if(m_inputActions.begin(), m_inputActions.end(), [actionName](InputAction* action) {
    return action->m_name == actionName;
  }), m_inputActions.end());

  m_inputActionsState.erase(inputActionStateIt);
}

bool InputModule::isActionActive(const std::string& actionName) const
{
  return m_inputActionsState.at(actionName) == InputActionState::Active;
}

void InputModule::enableGlobalTracking()
{
  SDL_CaptureMouse(SDL_TRUE);
}

void InputModule::disableGlobalTracking()
{
  SDL_CaptureMouse(SDL_FALSE);
}

bool InputModule::isGlobalTrackingEnabled() const
{
  uint32_t windowFlags = SDL_GetWindowFlags(m_window);

  return windowFlags & SDL_WINDOW_MOUSE_CAPTURE;
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
  MousePosition position{};
  SDL_GetMouseState(&position.x, &position.y);

  return position;
}

MousePosition InputModule::getMouseDelta() const
{
  MousePosition position{};
  SDL_GetRelativeMouseState(&position.x, &position.y);

  return position;
}

void InputModule::processRawSDLEvent(const SDL_Event& ev)
{
  if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
    KeyboardEvent event{};

    event.type = (ev.type == SDL_KEYDOWN) ? KeyboardEventType::KeyDown : KeyboardEventType::KeyUp;
    event.keyCode = ev.key.keysym.sym;
    event.repeated = ev.key.repeat;
    event.keyModifiers = SDL_GetModState();

    for (const auto& eventsListener : m_eventsListeners) {
      eventsListener->processKeyboardEvent(event);
    }

    InputActionState newState = (ev.type == SDL_KEYDOWN) ? InputActionState::Active : InputActionState::Inactive;
    toggleActionState(KeyboardInputAction(ev.key.keysym.sym), newState);
  }
  else if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
    MouseButtonEvent event{};

    event.type = (ev.type == SDL_MOUSEBUTTONDOWN) ? MouseButtonEventType::ButtonDown : MouseButtonEventType::ButtonUp;
    event.button = ev.button.button;

    for (const auto& eventsListener : m_eventsListeners) {
      eventsListener->processMouseButtonEvent(event);
    }

    InputActionState newState =
      (ev.type == SDL_MOUSEBUTTONDOWN) ? InputActionState::Active : InputActionState::Inactive;

    toggleActionState(MouseButtonClickAction(ev.button.button), newState);
  }
  else if (ev.type == SDL_MOUSEMOTION) {
    MouseMoveEvent event{};
    event.x = ev.motion.x;
    event.y = ev.motion.y;

    event.deltaX = ev.motion.xrel;
    event.deltaY = ev.motion.yrel;

    for (const auto& eventsListener : m_eventsListeners) {
      eventsListener->processMouseMoveEvent(event);
    }
  }
  else if (ev.type == SDL_MOUSEWHEEL) {
    MouseWheelEvent event{};
    event.wheelDelta = ev.wheel.y;

    for (const auto& eventsListener : m_eventsListeners) {
      eventsListener->processMouseWheelEvent(event);
    }
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

void InputModule::toggleActionState(const InputAction& action, InputActionState state)
{
  for (InputAction* currentAction : m_inputActions) {
    if (currentAction->equals(&action)) {
      m_inputActionsState[currentAction->m_name] = state;

      InputActionToggleEvent event;
      event.actionName = currentAction->m_name;
      event.newState = state;

      for (const auto& eventsListener : m_eventsListeners) {
        eventsListener->processInputActionToggleEvent(event);
      }
    }
  }
}

bool InputModule::isMouseButtonPressed(uint8_t button) const
{
  return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

InputAction::InputAction() = default;

InputAction::~InputAction() = default;

KeyboardInputAction::KeyboardInputAction(SDL_Keycode keyCode)
  : m_keyCode(keyCode)
{

}

SDL_Keycode KeyboardInputAction::getKeyCode() const
{
  return m_keyCode;
}

InputAction* KeyboardInputAction::clone() const
{
  return new KeyboardInputAction(m_keyCode);
}

bool KeyboardInputAction::equals(const InputAction* action) const
{
  const auto* keyboardAction = dynamic_cast<const KeyboardInputAction* const>(action);
  return keyboardAction != nullptr && keyboardAction->getKeyCode() == m_keyCode;
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

bool MouseButtonClickAction::equals(const InputAction* action) const
{
  const auto* clickAction = dynamic_cast<const MouseButtonClickAction* const>(action);
  return clickAction != nullptr && clickAction->getButtonCode() == m_button;
}
