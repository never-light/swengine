#include "Font.h"

Font::Font()
	: m_bitmap(nullptr), m_baseSize(0)
{
}

Font::Font(Texture* bitmap)
	: m_bitmap(bitmap), m_baseSize(0)
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
