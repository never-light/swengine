#pragma once

#include <variant>

#include "InputActions.h"

using InputActionToggleEventArgs = std::variant<KeyboardInputArgs, MouseButtonClickArgs>;

struct InputActionToggleEvent {
    std::string actionName;
    InputActionState newState;
    InputActionToggleEventArgs args;
};

struct MouseMoveEvent {
    int x;
    int y;

    int deltaX;
    int deltaY;
};

enum class InputEventType {
    InputActionToggle, MouseMove
};

struct InputEvent {
    std::variant<InputActionToggleEvent, MouseMoveEvent> eventData;
};

class InputEventsListener {
public:
    InputEventsListener() = default;
    virtual ~InputEventsListener() = default;

    virtual void processInputEvent(const InputEvent& event) = 0;
};
