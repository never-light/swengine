#pragma once

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

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

class InputModule {
 public:
  explicit InputModule(SDL_Window* window);
  ~InputModule();

  void registerAction(const std::string& actionName,
                      const InputAction& action);

  void unregisterAction(const std::string& actionName);

  [[nodiscard]] bool isActionActive(const std::string& actionName) const;

  void enableGlobalTracking();
  void disableGlobalTracking();

  [[nodiscard]] bool isGlobalTrackingEnabled() const;

  void setMouseMovementMode(MouseMovementMode mode);
  [[nodiscard]] MouseMovementMode getMouseMovementMode() const;

  void setMousePosition(const MousePosition& position);
  [[nodiscard]] MousePosition getMousePosition() const;

  [[nodiscard]] MousePosition getMouseDelta() const;

  void processRawSDLEvent(const SDL_Event& ev);

  void registerEventsListener(std::shared_ptr<InputEventsListener> listener);
  void unregisterEventsListener(std::shared_ptr<InputEventsListener> listener);

 private:
  void toggleActionState(const InputAction& action, InputActionState state);

 private:
  std::vector<InputAction*> m_inputActions;
  std::unordered_map<std::string, InputActionState> m_inputActionsState;

  std::vector<std::shared_ptr<InputEventsListener>> m_eventsListeners;

  SDL_Window* m_window;
};

