#include "SoundImporter.h"

#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QImage>

#include <LevelEditor/Components/Utils/filesystem.h>

#include "../SoundAsset.h"
#include "../../UI/AssetsManagerWindow.h"

SoundImporter::SoundImporter(AssetsManagerWindow* parentWindow)
	: AssetImporter(parentWindow)
{

}

SoundImporter::~SoundImporter() {

}

AssetBase* SoundImporter::importFromFile(const QString& fileName, const QString& targetFileName) {
	QFile sourceFile(fileName);
	if (!sourceFile.copy(targetFileName))
		throw new std::exception();

	SoundAsset* asset = new SoundAsset();
	asset->setFileName(QDir(getEditorAssetsDir()).relativeFilePath(targetFileName));
	asset->setName(QFileInfo(sourceFile).baseName());

	QString message = "Sound is imported!";

	QMessageBox::information(m_parentWindow, "Sound import", message, QMessageBox::Ok);

	return asset;
}

QString SoundImporter::getExtension() const
{
	return "";
}
