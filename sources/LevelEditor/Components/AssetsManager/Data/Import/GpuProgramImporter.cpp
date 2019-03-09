#include "GpuProgramImporter.h"

#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QImage>

#include <LevelEditor/Components/Utils/filesystem.h>

#include "../GpuProgramAsset.h"
#include "../../UI/AssetsManagerWindow.h"

GpuProgramImporter::GpuProgramImporter(AssetsManagerWindow* parentWindow)
	: AssetImporter(parentWindow)
{

}

GpuProgramImporter::~GpuProgramImporter() {

}

AssetBase* GpuProgramImporter::importFromFile(const QString& fileName, const QString& targetFileName) {
	QFile sourceFile(fileName);
	if (!sourceFile.copy(targetFileName))
		throw new std::exception();

	GpuProgramAsset* asset = new GpuProgramAsset();
	asset->setFileName(QDir(getEditorAssetsDir()).relativeFilePath(targetFileName));
	asset->setName(QFileInfo(sourceFile).baseName());

	QString message = "Gpu program is imported!";

	QMessageBox::information(m_parentWindow, "Gpu program import", message, QMessageBox::Ok);

	return asset;
}

QString GpuProgramImporter::getExtension() const
{
	return "fx";
}
