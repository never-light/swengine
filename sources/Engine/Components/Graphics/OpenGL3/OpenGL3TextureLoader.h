#pragma once

#include <Engine\Components\Graphics\RenderSystem\TextureLoader.h>

class OpenGL3TextureLoader : public TextureLoader {
public:
	Texture* loadFromFile(const std::string& filename) override;
};