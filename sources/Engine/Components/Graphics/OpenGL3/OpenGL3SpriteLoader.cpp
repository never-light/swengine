#include "OpenGL3SpriteLoader.h"
#include "OpenGL3Sprite.h"

Sprite* OpenGL3SpriteLoader::createSprite() {
	return new OpenGL3Sprite();
}