#include "precompiled.h"

#pragma hdrstop

#include "BitmapFont.h"

BitmapFont::BitmapFont(ResourceHandle<GLTexture> bitmap, const std::array<BitmapCharacter, 256>& characters,
  int baseSize, int height)
  : m_bitmap(bitmap),
    m_characters(characters),
    m_baseSize(baseSize),
    m_height(height)
{
}

const BitmapCharacter& BitmapFont::getCharacter(unsigned char character) const
{
  return m_characters[character];
}

GLTexture* BitmapFont::getBitmap() const
{
  return m_bitmap.get();
}

int BitmapFont::getBaseSize() const
{
  return m_baseSize;
}

int BitmapFont::getHeight() const
{
  return m_height;
}

ResourceHandle<GLTexture> BitmapFont::getBitmapResource() const
{
  return m_bitmap;
}
