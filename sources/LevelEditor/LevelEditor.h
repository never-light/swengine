#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LevelEditor.h"

#include "Components/AssetsManager/AssetsManager.h"

class LevelEditor : public QMainWindow {
	Q_OBJECT

public:
	LevelEditor(QWidget *parent = Q_NULLPTR);
	~LevelEditor();

public slots:
	void openAssetsManager();

private:
	Ui::LevelEditorClass ui;

	AssetsManager* m_assetsManager;
};
