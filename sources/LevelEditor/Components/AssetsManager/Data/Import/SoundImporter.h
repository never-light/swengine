#pragma once

#include "../AssetImporter.h"

class SoundImporter final : public AssetImporter {
public:
	SoundImporter(AssetsManagerWindow* parentWindow);
	~SoundImporter();

	virtual AssetBase* importFromFile(const QString& fileName, const QString& targetFileName);
	virtual QString getExtension() const;

private:

};