#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include <SDL.h>
#include <glm/vec2.hpp>

#include "Modules/ECS/ECS.h"
#include "InputEvents.h"

struct MousePosition {
    int x;
    int y;
};

enum class MouseMovementMode {
    Absolute, Relative
};

class InputModule
{
public:
    InputModule(SDL_Window* window);
    ~InputModule();

    void registerAction(const std::string& actionName,
                        const InputAction& action);

    bool isActionActive(const std::string& actionName) const;

    void setMouseMovementMode(MouseMovementMode mode);
    MouseMovementMode getMouseMovementMode() const;

    void setMousePosition(const MousePosition& position);
    MousePosition getMousePosition() const;

    MousePosition getMouseDelta() const;

    void processRawSDLEvent(const SDL_Event& ev);

    void registerEventsListener(std::shared_ptr<InputEventsListener> listener);
    void unregisterEventsListener(std::shared_ptr<InputEventsListener> listener);

private:
    void toggleActionState(const InputAction& action, InputActionState state, const InputActionToggleEventArgs& args);
    void triggerInputEvent(const InputEvent& event);

private:
    std::vector<InputAction*> m_inputActions;
    std::unordered_map<std::string, InputActionState> m_inputActionsState;

    std::vector<std::shared_ptr<InputEventsListener>> m_eventsListeners;

    SDL_Window* m_window;
};

