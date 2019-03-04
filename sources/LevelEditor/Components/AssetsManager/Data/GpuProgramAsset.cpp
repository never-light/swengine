#include "GpuProgramAsset.h"

GpuProgramAsset::GpuProgramAsset(const QString & fileName)
	: FileAsset(fileName)
{
	m_typeProperty->setValue("Gpu program");
}

GpuProgramAsset::~GpuProgramAsset()
{
}
