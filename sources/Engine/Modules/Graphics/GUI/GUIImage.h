#pragma once

#include <memory>

#include "GUIWidget.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

class GUIImage : public GUIWidget {
 public:
  explicit GUIImage(std::shared_ptr<GLTexture> image);

  inline void setImage(std::shared_ptr<GLTexture> image,
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default)
  {
    return getVisualParameters(visualState).setBackgroundImage(std::move(image));
  }

  [[nodiscard]] inline std::shared_ptr<GLTexture> getImage(
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const
  {
    return getVisualParameters(visualState).getBackgroundImage();
  }

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex) override;
};
