#include "MeshImporter.h"

#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QPlainTextEdit>
#include <QMessageBox>

#include <MeshImporter/MeshImporter.h>
#include <LevelEditor/Components/Utils/filesystem.h>

#include <QDir>

#include "../../UI/AssetsManagerWindow.h"
#include "../MeshAsset.h"

MeshAssetImporter::MeshAssetImporter(AssetsManagerWindow* parentWindow) 
	: AssetImporter(parentWindow) 
{

}

MeshAssetImporter::~MeshAssetImporter() {

}

AssetBase* MeshAssetImporter::importFromFile(const QString& fileName, const QString& targetFileName) {
	QDialog meshImportDialog((QWidget*)m_parentWindow);

	Ui::MeshImportDialog meshImportDialogUi;
	meshImportDialogUi.setupUi(&meshImportDialog);
	
	meshImportDialogUi.selectedFileName->setText(fileName);
	meshImportDialogUi.targetFileName->setText(targetFileName);

	meshImportDialog.setModal(true);
	int dialogResult = meshImportDialog.exec();

	if (dialogResult != 1)
		return nullptr;
	
	MeshImporter::MeshImportOptionsList options;
	options.set(MeshImporter::MeshImportOptions::CalculateTangents);
	options.set(MeshImporter::MeshImportOptions::FlipUV);
	options.set(MeshImporter::MeshImportOptions::GroupByMaterials);
	options.set(MeshImporter::MeshImportOptions::WeldIdenticalVertices);
	
	MeshImporter::MeshImporter importer(fileName.toStdString(), options);
	importer.saveToFile(targetFileName.toStdString());

	for (std::string str : importer.getLog())
		m_parentWindow->getLogWidget()->appendPlainText(QString::fromStdString(str));

	//qDebug() << QVector(importer.getLog());

	MeshAsset* asset = new MeshAsset();
	asset->setFileName(QDir(getEditorAssetsDir()).relativeFilePath(targetFileName));
	asset->setName(QFileInfo(fileName).baseName());
	asset->setVerticesCount(importer.getVerticesCount());
	asset->setIndicesCount(importer.getIndicesCount());
	asset->setSubMeshesCount(importer.getSubMeshesCount());
	asset->setSize(QFile(targetFileName).size());

	QString message = "Mesh is imported!\n";
	message += "Vertices: " + QString::number(importer.getVerticesCount()) + "\n";
	message += "Indices: " + QString::number(importer.getIndicesCount()) + "\n";
	message += "SubMeshes: " + QString::number(importer.getSubMeshesCount());

	QMessageBox::information(m_parentWindow, "Mesh import", message, QMessageBox::Ok);

	return asset;
}

QString MeshAssetImporter::getExtension() const
{
	return "mesh";
}
