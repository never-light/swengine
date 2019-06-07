#pragma once

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
#include "Engine/Components/Platform/Windows/W32Cursor.h"

using Cursor = sw::platform::base::Cursor;
using CursorBehaviour = sw::platform::base::Window::CursorBehaviour;

class GUIManager : public InputEventsListener {
public:
	GUIManager(std::shared_ptr<sw::platform::base::Window> window, 
		InputManager* inputManager, 
		GraphicsContext* graphicsContext,
		GpuProgram* guiGpuProgram);
	virtual ~GUIManager();

	virtual void render();
	virtual void update();

	GUILayout* getMainLayout() const;

	void setCursorBehaviour(CursorBehaviour behaviour);
	CursorBehaviour getCursorBehaviour() const;

	void resetCurrentCursor();
	void setCurrentCursor(std::shared_ptr<Cursor> cursor);
	Cursor* getCurrentCursor() const; 

protected:
	void onMouseButtonPress(MouseButton button) override;
	void onKeyPress(KeyboardKey key) override;
	void onCharacterEntered(unsigned char character) override;
	void onKeyRepeat(KeyboardKey key) override;


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
	std::shared_ptr<sw::platform::base::Window> m_window;
	std::shared_ptr<sw::platform::base::Cursor> m_defaultCursor;

	InputManager* m_inputManager;
	GraphicsContext* m_graphicsContext;
};