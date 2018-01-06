#pragma once

#include <string>
#include "GpuProgram.h"

class GpuProgramLoader {
public:
	virtual GpuProgram* loadFromFile(const std::string& filename) = 0;
};