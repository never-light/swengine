#pragma once

#include <memory>

#include "Modules/ECS/ECS.h"
#include "InputModule.h"

class InputSystem : public GameSystem, public InputEventsListener, public std::enable_shared_from_this<InputSystem> {
 public:
  explicit InputSystem(std::shared_ptr<InputModule> inputModule);
  ~InputSystem() override = default;

  void configure() override;
  void unconfigure() override;

  void processInputActionToggleEvent(const InputActionToggleEvent& event) override;
  void processMouseMoveEvent(const MouseMoveEvent& event) override;
  void processKeyboardEvent(const KeyboardEvent& event) override;
  void processMouseButtonEvent(const MouseButtonEvent& event) override;
  void processMouseWheelEvent(const MouseWheelEvent& event) override;

 private:
  std::shared_ptr<InputModule> m_inputModule;

};

