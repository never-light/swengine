#pragma once

#include <Engine\Components\GUI\GUIManager.h>
#include <Game\Game\HUD\HUDWindow.h>

#include "CodePanel.h"

class GameHUD {
public:
	GameHUD(GraphicsContext* graphicsContext, 
		Font* defaultFont, GUIManager* guiManager, GUILayout* guiLayout);
	~GameHUD();

	void update();

	void setCurrentTaskInfo(const std::string& taskTitle, const std::string& objectiveTitle);
	void setNoActiveTask();
	void showTaskInfo();
	void hideTaskInfo();

	void setInteractiveObjectHint(const std::string& hint);
	void showInteractiveObjectHint();
	void hideInteractiveObjectHint();

	void registerModalWindow(HUDWindow* window);

	bool isControlLocked() const;

	void lockControl();
	void unlockControl();

	unsigned int getScreenWidth() const;
	unsigned int getScreenHeight() const;

	Font* getDefaultFont() const;

	CodePanel* getCodePanelWindow() const;

private:
	void openModalWindowCallback(HUDWindow* window);
	void closeModalWindowCallback(HUDWindow* window);

private:
	bool m_isControlLocked;
	bool m_isActiveTaskExists;

private:
	CodePanel * m_codePanel;

private:
	GUIText* m_currentTaskText;
	GUIText* m_currentObjectiveText;

	GUIText* m_interactiveHint;

	HUDWindow* m_activeWindow;

private:
	Font * m_defaultFont;
	GUILayout * m_guiLayout;

	GUIManager* m_guiManager;
	GraphicsContext* m_graphicsContext;
};