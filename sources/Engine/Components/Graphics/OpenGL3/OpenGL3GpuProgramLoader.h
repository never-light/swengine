#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgramLoader.h>

class OpenGL3GpuProgramLoader : public GpuProgramLoader {
public:
	GpuProgram* loadFromFile(const std::string& filename) override;
};