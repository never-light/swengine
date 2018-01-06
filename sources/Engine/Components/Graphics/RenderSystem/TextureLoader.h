#pragma once

#include <string>
#include "Texture.h"

class TextureLoader {
public:
	virtual Texture* loadFromFile(const std::string& filename) = 0;
};