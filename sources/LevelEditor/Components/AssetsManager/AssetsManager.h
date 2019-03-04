#pragma once

#include "UI/AssetsManagerWindow.h"
#include "AssetsDataBase.h"

class AssetsManager {
public:
	AssetsManager(QWidget* mainWindow, const QString& categoriesPath, const QString& assetsPath);
	~AssetsManager();

	AssetsManagerWindow* getWindow() const;
	void show();

private:
	AssetsManagerWindow* m_assetsManagerWindow;
	AssetsDataBase* m_assetsDataBase;
};