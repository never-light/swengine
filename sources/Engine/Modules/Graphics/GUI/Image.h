#pragma once

#include <memory>

#include "GUIWidget.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

class Image : public GUIWidget
{
public:
    Image(std::shared_ptr<GLTexture> image);

    void render(GUISystem& guiSystem);
};
