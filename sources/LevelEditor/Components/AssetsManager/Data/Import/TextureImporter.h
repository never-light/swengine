#pragma once

#include "../AssetImporter.h"

class TextureImporter final : public AssetImporter {
public:
	TextureImporter(AssetsManagerWindow* parentWindow);
	~TextureImporter();

	virtual AssetBase* importFromFile(const QString& fileName, const QString& targetFileName);
	virtual QString getExtension() const;

private:

};