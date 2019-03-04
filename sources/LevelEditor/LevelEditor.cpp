#include "LevelEditor.h"

LevelEditor::LevelEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_assetsManager = new AssetsManager(this, "../editor/assets/categories.xml", "../editor/assets/assets.xml");

	connect(ui.MainMenu_Assets_Manager, SIGNAL(triggered()), this, SLOT(openAssetsManager()));
}

void LevelEditor::openAssetsManager() {
	m_assetsManager->show();
}

LevelEditor::~LevelEditor()
{
	delete m_assetsManager;
}
