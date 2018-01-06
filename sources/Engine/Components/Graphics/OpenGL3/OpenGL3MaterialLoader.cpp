#include "OpenGL3MaterialLoader.h"
#include "OpenGL3Material.h"

Material* OpenGL3MaterialLoader::createMaterial() {
	return new OpenGL3Material();
}