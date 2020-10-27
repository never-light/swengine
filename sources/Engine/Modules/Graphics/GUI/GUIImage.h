#pragma once

#include <memory>

#include "GUIWidget.h"

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

class GUIImage : public GUIWidget {
 public:
  explicit GUIImage(ResourceHandle<GLTexture> image);

  inline void setImage(ResourceHandle<GLTexture> image,
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default)
  {
    return getVisualParameters(visualState).setBackgroundImage(image);
  }

  [[nodiscard]] inline ResourceHandle<GLTexture> getImage(
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const
  {
    return getVisualParameters(visualState).getBackgroundImage().value();
  }

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;
};
