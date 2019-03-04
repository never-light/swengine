#include "TextureImporter.h"

#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QImage>

#include <LevelEditor/Components/Utils/filesystem.h>

#include "../TextureAsset.h"
#include "../../UI/AssetsManagerWindow.h"

TextureImporter::TextureImporter(AssetsManagerWindow* parentWindow)
	: AssetImporter(parentWindow)
{

}

TextureImporter::~TextureImporter() {

}

AssetBase* TextureImporter::importFromFile(const QString& fileName, const QString& targetFileName) {
	QFile sourceFile(fileName);
	if (!sourceFile.copy(targetFileName))
		throw new std::exception();

	QImage texture(targetFileName);

	TextureAsset* asset = new TextureAsset(QDir(getEditorAssetsDir()).relativeFilePath(targetFileName));
	asset->setName(QFileInfo(sourceFile).baseName());
	asset->setFileSize(sourceFile.size());
	asset->setSize(texture.size());

	QString message = "Texture is imported!\n";
	message += "Size: " + QString::number(texture.size().width()) + "x" + QString::number(texture.size().height()) + "\n";
	message += "File size: " + QString::number(sourceFile.size()) + " bytes";

	QMessageBox::information(m_parentWindow, "Texture import", message, QMessageBox::Ok);

	return asset;
}

QString TextureImporter::getExtension() const
{
	return "";
}
