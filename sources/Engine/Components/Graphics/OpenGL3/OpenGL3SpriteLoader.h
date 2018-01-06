#pragma once

#include <Engine\Components\Graphics\RenderSystem\SpriteLoader.h>

class OpenGL3SpriteLoader : public SpriteLoader {
public:
	Sprite* createSprite() override;
};