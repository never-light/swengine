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

    virtual InputAction* clone() const = 0;
    virtual bool equals(const InputAction* const action) const = 0;

private:
    friend class InputModule;

private:
    std::string m_name;
};

class KeyboardInputAction : public InputAction {
public:
    KeyboardInputAction(SDL_Keycode keyCode);

    SDL_Keycode getKeyCode() const;

    InputAction* clone() const override;
    bool equals(const InputAction* const action) const override;

private:
    SDL_Keycode m_keyCode;
};

class MouseButtonClickAction : public InputAction {
public:
    MouseButtonClickAction(uint8_t button);

    uint8_t getButtonCode() const;

    InputAction* clone() const override;
    bool equals(const InputAction* const action) const override;

private:
    uint8_t m_button;
};
