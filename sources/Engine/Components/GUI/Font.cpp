#include "Font.h"

#include <Engine/assertions.h>

Font::Font()
	: m_bitmap(nullptr), 
	m_baseSize(0),
	m_height(0)
{
}

Font::Font(Texture* bitmap)
	: m_bitmap(bitmap),
	m_baseSize(0),
	m_height(0)
{

}

Font::~Font()
{
	if (m_bitmap != nullptr) {
		delete m_bitmap;
		m_bitmap = nullptr;
	}
}

void Font::addCharacter(unsigned char character, const Character& description)
{
	m_characters[character] = description;
}

Character Font::getCharacter(unsigned char character) const
{
	return m_characters[character];
}

void Font::setBitmap(Texture* texture)
{
	_assert(texture != nullptr);

	m_bitmap = texture;
}

Texture * Font::getBitmap() const
{
	return m_bitmap;
}

unsigned int Font::getBaseSize() const
{
	return m_baseSize;
}

void Font::setBaseSize(unsigned int size)
{
	m_baseSize = size;
}

unsigned int Font::getHeight() const
{
	return m_height;
}

void Font::setHeight(unsigned int height)
{
	m_height = height;
}
