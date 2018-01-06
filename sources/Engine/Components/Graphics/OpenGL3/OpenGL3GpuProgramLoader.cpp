#include "OpenGL3GpuProgramLoader.h"
#include "OpenGL3Shader.h"

GpuProgram* OpenGL3GpuProgramLoader::loadFromFile(const std::string& filename) {
	return new OpenGL3GpuProgram(filename);
}