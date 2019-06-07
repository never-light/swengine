#pragma once

#include <vector>

#include <Engine/types.h>
#include <Engine/Components/Math/Math.h>

#include <Engine/Components/Platform/Base/BaseWindow.h>
#include "InputEventsListener.h"

/*!
 * \brief Class for user input management
 * 
 */
class InputManager : public sw::platform::base::EventsListener {
public:
	InputManager(std::shared_ptr<sw::platform::base::Window> window);
	~InputManager();

	void initialize();
	void update();

	/*!
	 * \brief Registers an event listener in the input management system
	 * 
	 * \param listener
	 */
	void registerEventListener(InputEventsListener* listener);

	/*!
	 * \brief Removes an event listener from the input management system
	 *
	 * \param listener
	 */
	void unregisterEventListener(InputEventsListener* listener);

	/*!
	 * \brief Get current keyboard key state
	 * 
	 * \param key
	 * \return The current state of the specified key
	 */
	KeyboardKeyState getKeyState(KeyboardKey key) const;

	/*!
	 * \brief Checks whether the key is pressed
	 * 
	 * \param key
	 * \return 
	 */
	bool isKeyPressed(KeyboardKey key) const;

	/*!
	 * \brief Checks whether the key is released
	 *
	 * \param key
	 * \return
	 */
	bool isKeyReleased(KeyboardKey key) const;

	CursorPosition getMousePosition() const;
	MouseButtonState getMouseButtonState(MouseButton button) const;
	MouseState getMouseState() const;

	void onKeyPress(KeyboardKey key) override;
	void onKeyRelease(KeyboardKey key) override;
	void onMouseButtonPress(MouseButton button) override;
	void onMouseButtonRelease(MouseButton button) override;
	void onMouseMove(const CursorPosition& position) override;

private:
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onCharacterEnteredEvent(unsigned int character);
	void onMouseMovedEvent(double x, double y);
	void onMouseButtonEvent(int button, int action, int mods);
	void onScrollEvent(double offsetX, double offsetY);

private:
	std::shared_ptr<sw::platform::base::Window> m_window;

	std::vector<InputEventsListener*> m_eventListeners;
};