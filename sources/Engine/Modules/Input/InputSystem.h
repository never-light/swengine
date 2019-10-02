//#pragma once

//#include "Modules/ECS/GameSystem.h"
//#include "Modules/Math/vector2.h"
//#include <QWidget>
//#include <QMap>

//enum class KeyboardKeyState {
//    Pressed, Released
//};

//enum class MouseButtonState {
//    Pressed, Released
//};

//class InputSystem : public GameSystem
//{
//public:
//    InputSystem();
//    ~InputSystem() override;

//    /*!
//     * \brief The function is called on the Application start
//     * \param gameWorld
//     */
//    void configure(GameWorld *gameWorld) override;

//    /*!
//     * \brief The function is called ON the Application shutdown
//     * \param gameWorld
//     */
//    void unconfigure(GameWorld *gameWorld) override;

//    /*!
//     * \brief The function is called when the MainWindow receives KeyDown event
//     * \param key
//     */
//    void notifyKeyDown(Qt::Key key);

//    /*!
//     * \brief The function is called when the MainWindow receives KeyUp event
//     * \param key
//     */
//    void notifyKeyUp(Qt::Key key);

//    /*!
//     * \brief The function is called when the MainWindow receives MouseButtonDown event
//     * \param button
//     */
//    void notifyMouseButtonDown(Qt::MouseButton button);

//    /*!
//     * \brief The function is called when the MainWindow receives MouseButtonUp event
//     * \param button
//     */
//    void notifyMouseButtonUp(Qt::MouseButton button);

//    /*!
//     * \brief The function is called when the MainWindow receives MouseMove event
//     * \param position New cursor position
//     */
//    void notifyMouseMove(QPoint position);

//    /*!
//     * \brief Checks whether the keyboard key is down
//     * \param key
//     * \return Key down flag
//     */
//    bool isKeyDown(Qt::Key key) const;

//    /*!
//     * \brief Checks whether the keyboard key is up
//     * \param key
//     * \return Key up flag
//     */
//    bool isKeyUp(Qt::Key key) const;

//    /*!
//     * \brief Checks whether the mouse button is down
//     * \param button
//     * \return Mouse button down flag
//     */
//    bool isMouseButtonDown(Qt::MouseButton button) const;

//    /*!
//     * \brief Checks whether the mouse button is down
//     * \param button
//     * \return Mouse button up flag
//     */
//    bool isMouseButtonUp(Qt::MouseButton button) const;

//    /*!
//     * \brief getCursorPosition Returns current cursor position
//     * \return Cusror position in window coordinate system
//     */
//    vector2 getCursorPosition() const;

//private:
//    QMap<Qt::Key, KeyboardKeyState> m_keyboardKeysState;
//    QMap<Qt::MouseButton, MouseButtonState> m_mouseButtonsState;

//    vector2 m_cursorPosition;
//};

