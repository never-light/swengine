#pragma once

#include <string>
#include "Sprite.h"

class SpriteLoader {
public:
	virtual Sprite* createSprite() = 0;
};