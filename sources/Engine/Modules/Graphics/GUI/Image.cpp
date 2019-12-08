#include "Image.h"
#include "GUISystem.h"

Image::Image(std::shared_ptr<GLTexture> image)
{
    setBackgroundImage(image);
    setHoverBackgroundImage(image);
}

void Image::render(GUISystem& guiSystem)
{
    RenderTask task = guiSystem.getRenderTaskTemplate(this);
    guiSystem.getGraphicsContext()->executeRenderTask(task);
}
