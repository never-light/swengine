#pragma once

#include "RawImage.h"
#include <unordered_map>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>
#include <Engine\Components\Math\Geometry\Rect.h>

struct Character {
public:
	Rect rectangleInAtlas;
	vector2 uv;

	int xOffset;
	int yOffset;
	int xAdvance;
};

class Font {
public:
	Font();
	Font(Texture* bitmap);
	~Font();

	void addCharacter(unsigned char character, const Character& description);
	Character getCharacter(unsigned char character) const;

	void setBitmap(Texture* data);
	Texture* getBitmap() const;

	unsigned int getBaseSize() const;
	void setBaseSize(unsigned int size);

	unsigned int getHeight() const;
	void setHeight(unsigned int height);
protected:
	Texture* m_bitmap;
	unsigned int m_baseSize;

	Character m_characters[256];

	unsigned int m_height;
};