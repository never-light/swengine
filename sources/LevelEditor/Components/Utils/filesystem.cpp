#include "filesystem.h"

QString getEditorRootDir()
{
	QDir rootDir = QDir::current();
	rootDir.cdUp();

	return rootDir.path();
}

QString getEditorAssetsDir()
{
	QDir rootDir = QDir::current();
	rootDir.cdUp();

	return rootDir.path() + QString("/") + EDITOR_DATA_ASSETS_DIR;
}
