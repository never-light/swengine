#include "precompiled.h"

#pragma hdrstop

#include "BitmapFont.h"

BitmapFont::BitmapFont(std::shared_ptr<GLTexture> bitmap, const std::array<BitmapCharacter, 256>& characters,
                       int baseSize, int height)
    : m_bitmap(bitmap),
      m_characters(characters),
      m_baseSize(baseSize),
      m_height(height) {
}

const BitmapCharacter& BitmapFont::getCharacter(unsigned char character) const {
  return m_characters[character];
}

std::shared_ptr<GLTexture> BitmapFont::getBitmap() const {
  return m_bitmap;
}

int BitmapFont::getBaseSize() const {
  return m_baseSize;
}

int BitmapFont::getHeight() const {
  return m_height;
}
