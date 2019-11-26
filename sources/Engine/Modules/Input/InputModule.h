#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <SDL.h>

class InputModule;

enum class InputActionState {
    Active, Inactive
};

struct InputActionArgs {
    InputActionArgs();
    virtual ~InputActionArgs();
};

enum class InputActionHandleMode {
    OnActivation, OnDeactivation, Always
};

class InputAction {
public:
    InputAction();
    virtual ~InputAction();

    virtual InputAction* clone() const = 0;
    virtual bool equals(const InputAction* const action) const = 0;

private:
    friend class InputModule;

protected:
    InputActionHandleMode m_handleMode = InputActionHandleMode::OnActivation;

private:
    std::string m_name;
    std::function<void(const InputActionArgs* const)> m_handler = nullptr;
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

struct KeyboardInputArgs : public InputActionArgs {
    SDL_Keycode keyCode;
    bool repeated;
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

struct MouseButtonClickArgs : public InputActionArgs {
    uint8_t button;
};

struct MousePosition {
    int x;
    int y;
};

struct MoveMoveArgs {
    MousePosition position;
};

class InputModule
{
public:
    InputModule();
    ~InputModule();

    void registerAction(const std::string& actionName,
                        const InputAction& action,
                        const std::function<void(const InputActionArgs* const)>& handler = nullptr);
    bool isActionActive(const std::string& actionName) const;

    void registerMouseMoveHandler(const std::function<void(const MoveMoveArgs* const)>& handler);
    MousePosition getMousePosition() const;

    void processRawSDLEvent(const SDL_Event& ev);

private:
    void toggleActionState(const InputAction& action, InputActionState state, const InputActionArgs* const args);

private:
    std::vector<InputAction*> m_inputActions;
    std::unordered_map<std::string, InputActionState> m_inputActionsState;

    std::vector<std::function<void(const MoveMoveArgs* const)>> m_mouseMoveHandlers;
};

