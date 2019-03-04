#pragma once

#include "../AssetImporter.h"
#include "ui_MeshImportDialog.h"

class MeshAssetImporter final : public AssetImporter {
public:
	MeshAssetImporter(AssetsManagerWindow* parentWindow);
	~MeshAssetImporter();

	virtual AssetBase* importFromFile(const QString& fileName, const QString& targetFileName);
	virtual QString getExtension() const;

private:
	
};