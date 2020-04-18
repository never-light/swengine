#include "precompiled.h"

#pragma hdrstop

#include "GUIImage.h"
#include "GUISystem.h"

GUIImage::GUIImage(std::shared_ptr<GLTexture> image)
{
  setBackgroundImage(image);
  setHoverBackgroundImage(image);
}
