#pragma once

#include <Engine/Components/GUI/GUI.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include <Engine/Components/InputManager/InputManager.h>

#include <vector>
#include "GUIWidget.h"
#include "GUILayout.h"

#include <Engine/Components/GUI/Widgets/GUIImage.h>
#include <Engine/Components/GUI/Widgets/GUIButton.h>
#include <Engine/Components/GUI/Widgets/GUIText.h>
#include <Engine/Components/GUI/Widgets/GUIWindow.h>
#include <Engine/Components/GUI/Widgets/GUIBlock.h>
#include <Engine/Components/GUI/Widgets/GUITextBox.h>


class GUIManager : public InputEventsListener {
public:
	GUIManager(Window* window, 
		InputManager* inputManager, 
		GraphicsContext* graphicsContext,
		GpuProgram* guiGpuProgram);
	virtual ~GUIManager();

	virtual void render();
	virtual void update();

	GUILayout* getMainLayout() const;

	void setCursorMode(CursorMode mode);
	CursorMode getCursorMode() const;

	void resetCurrentCursor();
	void setCurrentCursor(Cursor* cursor);
	Cursor* getCurrentCursor() const; 

protected:
	virtual void onMouseButtonPress(MouseButton button, const MouseState&) override;
	virtual void onKeyPress(Key key, KeyEvent::Modifier mod) override;
	virtual void onCharacterEntered(unsigned char character) override;
	virtual void onKeyRepeat(Key key, KeyEvent::Modifier mod) override;


	void onFocus(GUIWidget* widget);
protected:
	GUILayout* m_mainLayout;
	GUIWidget* m_focusedWidget;

	std::function<void(GUIWidget* widget)> m_onFocusCallback;

	matrix4 m_orthographicProjection;
protected:
	GeometryInstance * m_quad;
	GpuProgram* m_gpuProgram;

protected:
	Window * m_window;

	InputManager* m_inputManager;
	GraphicsContext* m_graphicsContext;

	Cursor* m_defaultCursor;
};