#pragma once

#include <Engine\Components\GUI\GUI.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include <Engine\Components\InputManager\InputManager.h>

#include <vector>
#include "GUIWidget.h"
#include "GUILayout.h"

#include <Engine\Components\GUI\Widgets\GUIImage.h>
#include <Engine\Components\GUI\Widgets\GUIButton.h>
#include <Engine\Components\GUI\Widgets\GUIText.h>

class GUIManager : public InputEventsListener {
public:
	GUIManager(Window* window, 
		InputManager* inputManager, 
		GraphicsContext* graphicsContext,
		GraphicsResourceFactory* graphicsResourceFactory,
		GpuProgram* guiGpuProgram);
	virtual ~GUIManager();

	virtual void render();
	virtual void update();

	GUILayout* getMainLayout() const;

	virtual void setCursorType(CursorType type);
	virtual CursorType getCursorType() const;
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
	GeometryStore * m_quad;
	GpuProgram* m_gpuProgram;

protected:
	Window * m_window;

	InputManager* m_inputManager;
	GraphicsContext* m_graphicsContext;
	GraphicsResourceFactory* m_graphicsResourceFactory;
};