#pragma once

#include <memory>
#include <array>

#include <glm/vec2.hpp>

#include "Modules/Math/Rect.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

struct BitmapCharacter {
 public:
  Rect bitmapArea;

  int xOffset;
  int yOffset;
  int xAdvance;

  bool isMapped = false;
};

class BitmapFont {
 public:
  BitmapFont(std::shared_ptr<GLTexture> bitmap, const std::array<BitmapCharacter, 256>& characters,
             int baseSize, int height);

  [[nodiscard]] const BitmapCharacter& getCharacter(unsigned char character) const;

  [[nodiscard]] std::shared_ptr<GLTexture> getBitmap() const;

  [[nodiscard]] int getBaseSize() const;
  [[nodiscard]] int getHeight() const;

 private:
  std::shared_ptr<GLTexture> m_bitmap;
  std::array<BitmapCharacter, 256> m_characters;

  int m_baseSize;
  int m_height;
};

