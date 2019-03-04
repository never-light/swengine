#include "AssetsManager.h"

AssetsManager::AssetsManager(QWidget* mainWindow, const QString& categoriesPath, const QString& assetsPath)
	: m_assetsManagerWindow(nullptr),
	m_assetsDataBase(new AssetsDataBase(categoriesPath, assetsPath))
{
	m_assetsManagerWindow = new AssetsManagerWindow(mainWindow, m_assetsDataBase);
}

AssetsManager::~AssetsManager()
{
	delete m_assetsManagerWindow;
	delete m_assetsDataBase;
}

AssetsManagerWindow * AssetsManager::getWindow() const
{
	return m_assetsManagerWindow;
}

void AssetsManager::show()
{
	m_assetsManagerWindow->show();
}
