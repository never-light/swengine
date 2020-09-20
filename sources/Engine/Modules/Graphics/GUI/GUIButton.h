#pragma once

#include <memory>

#include "GUIWidget.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

class GUIButton : public GUIWidgetRect {
 public:
  GUIButton();

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex) override;

};

