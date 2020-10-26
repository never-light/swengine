#pragma once

#include <memory>
#include <array>

#include <glm/vec2.hpp>

#include "Modules/Math/Rect.h"

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

struct BitmapCharacter {
 public:
  RectI bitmapArea{};

  int xOffset{};
  int yOffset{};
  int xAdvance{};
};

class BitmapFont : public Resource {
 public:
  BitmapFont(ResourceHandle<GLTexture> bitmap, const std::array<BitmapCharacter, 256>& characters,
    int baseSize, int height);
  ~BitmapFont() override = default;

  [[nodiscard]] const BitmapCharacter& getCharacter(unsigned char character) const;

  [[nodiscard]] GLTexture* getBitmap() const;

  [[nodiscard]] int getBaseSize() const;
  [[nodiscard]] int getHeight() const;

 private:
  ResourceHandle<GLTexture> m_bitmap;
  std::array<BitmapCharacter, 256> m_characters;

  int m_baseSize;
  int m_height;
};

