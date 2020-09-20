#include "precompiled.h"

#pragma hdrstop

#include "GUIImage.h"

#include <utility>
#include "GUISystem.h"

GUIImage::GUIImage(std::shared_ptr<GLTexture> image)
  : GUIWidget("image")
{
  getVisualParameters(GUIWidgetVisualState::Default).setBackgroundImage(std::move(image));
}

void GUIImage::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex)
{
  GUIWidget::applyStylesheetRule(stylesheetRule, selectorPartIndex);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "background") {
      // Background
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
        // Background image
        [this, visualState](const std::shared_ptr<GLTexture>& image) {
          this->setImage(image, visualState);
        },
      }, property.getValue());
    }
    else {
      SW_ASSERT(false);
    }

  });
}
