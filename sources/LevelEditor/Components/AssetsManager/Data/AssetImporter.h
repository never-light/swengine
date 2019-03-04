#pragma once

#include "AssetBase.h"
#include "AssetCategory.h"

class AssetsManagerWindow;

class AssetImporter {
public:
	AssetImporter(AssetsManagerWindow* parentWindow);
	virtual ~AssetImporter();

	virtual AssetBase* importFromFile(const QString& fileName, const QString& targetFileName) = 0;
	virtual QString getExtension() const = 0;

protected:
	AssetsManagerWindow* m_parentWindow;
};