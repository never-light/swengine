#include "precompiled.h"

#pragma hdrstop

#include "GUIImage.h"

#include <utility>
#include "GUISystem.h"

GUIImage::GUIImage(ResourceHandle<GLTexture> image)
  : GUIWidget("image")
{
  getVisualParameters(GUIWidgetVisualState::Default).setBackgroundImage(image);
}

void GUIImage::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidget::applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "background") {
      // Background
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
        // Background image
        [this, visualState](const ResourceHandle<GLTexture>& image) {
          this->setImage(image, visualState);
        },
      }, property.getValue());
    }
    else {
      SW_ASSERT(false);
    }

  });
}
