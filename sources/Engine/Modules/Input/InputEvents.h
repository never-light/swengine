#pragma once

#include <variant>
#include <string>

#include "InputActions.h"

struct InputActionToggleEvent {
  std::string actionName;
  InputActionState newState;
};

enum class KeyboardEventType {
  KeyDown, KeyUp
};

struct KeyboardEvent {
  KeyboardEventType type;
  SDL_Keycode keyCode;
  bool repeated;
  SDL_Keymod keyModifiers;
};

struct MouseMoveEvent {
  int x;
  int y;

  int deltaX;
  int deltaY;
};

enum class MouseButtonEventType {
  ButtonDown, ButtonUp
};

struct MouseButtonEvent {
  MouseButtonEventType type;
  uint8_t button;
};

struct MouseWheelEvent {
  int32_t wheelDelta;
};

class InputEventsListener {
 public:
  InputEventsListener() = default;
  virtual ~InputEventsListener() = default;

  virtual void processInputActionToggleEvent(const InputActionToggleEvent& event);
  virtual void processMouseMoveEvent(const MouseMoveEvent& event);
  virtual void processKeyboardEvent(const KeyboardEvent& event);
  virtual void processMouseButtonEvent(const MouseButtonEvent& event);
  virtual void processMouseWheelEvent(const MouseWheelEvent& event);
};
