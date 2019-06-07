#include "InputManager.h"

#include <Engine/assertions.h>

InputManager::InputManager(std::shared_ptr<sw::platform::base::Window> window) : m_window(window) {
	//m_window->addEventsListener(std::shared_ptr<EventsListener>{ this });
}

InputManager::~InputManager() {

}

void InputManager::initialize()
{
}

void InputManager::update() {

}

void InputManager::registerEventListener(InputEventsListener* listener) {
	_assert(listener != nullptr);

	m_eventListeners.push_back(listener);
}

void InputManager::unregisterEventListener(InputEventsListener* listener) {
	for (auto it = m_eventListeners.begin(); it != m_eventListeners.end(); it++) {
		if (*it == listener) {
			m_eventListeners.erase(it);
		}
	}
}

KeyboardKeyState InputManager::getKeyState(KeyboardKey key) const {
	return m_window->getKeyboardKeyState(key);
}

bool InputManager::isKeyPressed(KeyboardKey key) const {
	return getKeyState(key) == KeyboardKeyState::Pressed;
}

bool InputManager::isKeyReleased(KeyboardKey key) const {
	return getKeyState(key) == KeyboardKeyState::Released;
}

void InputManager::onKeyPress(KeyboardKey key)
{
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onKeyPress(key);
	}
}

void InputManager::onKeyRelease(KeyboardKey key)
{
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onKeyRelease(key);
	}
}

void InputManager::onMouseButtonPress(MouseButton button)
{
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onMouseButtonPress(button);
	}
}

void InputManager::onMouseButtonRelease(MouseButton button)
{
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onMouseButtonRelease(button);
	}
}

void InputManager::onMouseMove(const CursorPosition& position)
{
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onMouseMove(position);
	}
}

CursorPosition InputManager::getMousePosition() const {
	return m_window->getCursorPosition();
}

MouseButtonState InputManager::getMouseButtonState(MouseButton button) const {
	return m_window->getMouseButtonState(button);
}

MouseState InputManager::getMouseState() const {
	return m_window->getMouseState();
}

void InputManager::onCharacterEnteredEvent(unsigned int character)
{
	unsigned char ch = static_cast<unsigned char>(character);

	if (1040 <= character && character <= 1103) {
		ch = (unsigned char)'À' + (character - 1040);
	}
	else if (character > 255)
		return;

	for (InputEventsListener* listener : m_eventListeners) {
		listener->onCharacterEntered(ch);
	}
}

void InputManager::onScrollEvent(double offsetX, double offsetY) {
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onScroll(offsetX, offsetY);
	}
}