#pragma once

#include <SDL.h>
#include <string>

class InputModule;

enum class InputActionState {
  Active, Inactive
};

class InputAction {
 public:
  InputAction();
  virtual ~InputAction();

  [[nodiscard]] virtual InputAction* clone() const = 0;
  [[nodiscard]] virtual bool equals(const InputAction* action) const = 0;

 private:
  friend class InputModule;

 private:
  std::string m_name;
};

class KeyboardInputAction : public InputAction {
 public:
  explicit KeyboardInputAction(SDL_Keycode keyCode);

  [[nodiscard]] SDL_Keycode getKeyCode() const;

  [[nodiscard]] InputAction* clone() const override;
  [[nodiscard]] bool equals(const InputAction* action) const override;

 private:
  SDL_Keycode m_keyCode;
};

class MouseButtonClickAction : public InputAction {
 public:
  explicit MouseButtonClickAction(uint8_t button);

  [[nodiscard]] uint8_t getButtonCode() const;

  [[nodiscard]] InputAction* clone() const override;
  [[nodiscard]] bool equals(const InputAction* action) const override;

 private:
  uint8_t m_button;
};
