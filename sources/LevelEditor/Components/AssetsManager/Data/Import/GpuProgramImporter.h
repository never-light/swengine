#pragma once

#include "../AssetImporter.h"

class GpuProgramImporter final : public AssetImporter {
public:
	GpuProgramImporter(AssetsManagerWindow* parentWindow);
	~GpuProgramImporter();

	virtual AssetBase* importFromFile(const QString& fileName, const QString& targetFileName);
	virtual QString getExtension() const;
};