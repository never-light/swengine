#pragma once

#include <memory>

#include "GUIWidget.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

class GUIImage : public GUIWidget {
 public:
  explicit GUIImage(std::shared_ptr<GLTexture> image);
};
