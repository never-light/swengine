#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\GUI\GUILayout.h>
#include <Engine\Components\GUI\Widgets\GUIButton.h>
#include <Engine\Components\GUI\Widgets\GUIText.h>
#include <Engine\Components\GUI\Widgets\GUITextBox.h>

class MainMenu : public Scene {
public:
	enum class ButtonType {
		NewGame, Exit
	};

public:
	MainMenu(Window* window, GraphicsResourceFactory* graphicsResourceFactory, GraphicsContext* graphicsContext, ResourceManager* resourceManager, GUILayout* mainGUILayout);
	virtual ~MainMenu();

	virtual void update() override;
	virtual void render() override;

	virtual void activate() override;
	virtual void deactivate() override;

	void onButtonClick(ButtonType buttonType, const GUIButton::ClickCallback& callback);
protected:
	Window * m_window;

	GraphicsResourceFactory* m_graphicsResourceFactory;

	GUILayout* m_mainGUILayout;
	GUILayout* m_mainMenuGUILayout;

	GUIButton* m_newGameButton;
	GUIButton* m_exitButton;

	GUIText* m_text;
};