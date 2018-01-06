#include "OpenGL3TextureLoader.h"
#include "OpenGL3Texture.h"

Texture* OpenGL3TextureLoader::loadFromFile(const std::string& filename) {
	return new OpenGL3Texture(filename);
}