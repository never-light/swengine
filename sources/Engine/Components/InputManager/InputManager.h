#pragma once

#include <vector>

#include <Engine/types.h>
#include <Engine/Components/Math/Math.h>

#include <Engine/Components/GUI/Window.h>
#include "InputEvents.h"
#include "InputEventsListener.h"

/*!
 * \brief Class for user input management
 * 
 */
class InputManager {
public:
	InputManager(Window* window);
	~InputManager();

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
	KeyState getKeyState(Key key) const;

	/*!
	 * \brief Checks whether the key is pressed
	 * 
	 * \param key
	 * \return 
	 */
	bool isKeyPressed(Key key) const;

	/*!
	 * \brief Checks whether the key is released
	 *
	 * \param key
	 * \return
	 */
	bool isKeyReleased(Key key) const;

	/*!
	 * \brief Checks whether the key is repeated
	 *
	 * \param key
	 * \return
	 */
	bool isKeyRepeated(Key key) const;

	MousePosition getMousePosition() const;
	MouseButtonState getMouseButtonState(MouseButton button) const;
	MouseState getMouseState() const;
private:
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onCharacterEnteredEvent(unsigned int character);
	void onMouseMovedEvent(double x, double y);
	void onMouseButtonEvent(int button, int action, int mods);
	void onScrollEvent(double offsetX, double offsetY);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void mouseMovedCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
private:
	Window* m_window;

	std::vector<InputEventsListener*> m_eventListeners;
};