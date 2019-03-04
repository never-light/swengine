#pragma once

#include "FileAsset.h"

class GpuProgramAsset final : public FileAsset {
public:
	GpuProgramAsset(const QString& fileName);
	virtual ~GpuProgramAsset();
};