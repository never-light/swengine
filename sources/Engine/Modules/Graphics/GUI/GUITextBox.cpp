#include "precompiled.h"

#pragma hdrstop

#include "GUITextBox.h"

#include <utility>
#include "GUISystem.h"

GUITextBox::GUITextBox(ResourceHandle<BitmapFont> font)
  : GUIWidgetRect("textbox"),
    m_font(font),
    m_text(std::make_unique<GUIText>(font, ""))
{
}

void GUITextBox::setFont(ResourceHandle<BitmapFont> font)
{
  m_font = std::move(font);
}

ResourceHandle<BitmapFont> GUITextBox::getFont() const
{
  return m_font;
}

void GUITextBox::setText(const std::string& text)
{
  m_text->setText(text);
}

std::string GUITextBox::getText() const
{
  return m_text->getText();
}

void GUITextBox::setTextColor(const glm::vec4& color, GUIWidgetVisualState visualState)
{
  m_text->setColor(color, visualState);
}

glm::vec4 GUITextBox::getTextColor(GUIWidgetVisualState visualState) const
{
  return m_text->getColor(visualState);
}

void GUITextBox::setTextFontSize(int size)
{
  m_text->setFontSize(size);
}

int GUITextBox::getTextFontSize() const
{
  return m_text->getFontSize();
}

void GUITextBox::render(GUISystem& guiSystem)
{
  RenderTask task = guiSystem.getRenderTaskTemplate(this);
  guiSystem.getGraphicsContext()->scheduleRenderTask(task);

  m_text->render(guiSystem);
}

bool GUITextBox::canHaveFocus() const
{
  return true;
}

void GUITextBox::processKeyboardEvent(const GUIKeyboardEvent& event)
{
  if (event.type != KeyboardEventType::KeyDown) {
    return;
  }

  char character = static_cast<char>(event.keyCode);

  if (event.keyCode == SDLK_BACKSPACE) {
    std::string text = m_text->getText();

    if (!text.empty()) {
      text.pop_back();
      m_text->setText(text);
    }
  }
  else if (isprint((unsigned char)character)) {
    char printedCharacter = character;

    if (event.keyModifiers & KMOD_SHIFT) {
      printedCharacter = static_cast<char>(std::toupper(printedCharacter));
    }

    m_text->setText(m_text->getText() + printedCharacter);
  }
}

glm::mat4 GUITextBox::updateTransformationMatrix()
{
  glm::mat4 transformationMatrix = GUIWidget::updateTransformationMatrix();

  if (m_text->getParent() == nullptr) {
    addChildWidget(m_text);
  }

  if (!m_text->getText().empty()) {
    if (m_text->m_needTextGeometryUpdate) {
      m_text->updateAndGetGeometryStore();
    }

    m_text->setOrigin({10, getSize().y / 2 - m_text->getSize().y / 2});
  }

  return transformationMatrix;
}

void GUITextBox::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "text-color") {
      // Text color
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
        [this, visualState](const glm::vec4& color) {
          this->setTextColor(color, visualState);
        },
      }, property.getValue());
    }
    else if (propertyName == "font-size") {
      // Font size
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
        [this, visualState](int size) {
          SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Font-size is supported only for default state");

          this->setTextFontSize(size);
        },
      }, property.getValue());
    }
    else if (propertyName == "font-family") {
      // Font family
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
        [this, visualState](ResourceHandle<BitmapFont> font) {
          SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Font-family is supported only for default state");

          this->setFont(font);
        },
      }, property.getValue());
    }
    else if (propertyName == "background") {
      // Do nothing as property should be already processed by GUILayout
    }
    else {
      SW_ASSERT(false);
    }
  });

}

void GUITextBox::applyStylesheetRuleToChildren(
  const GUIWidgetStylesheetRule& stylesheetRule,
  const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath)
{
  GUIWidget::applyStylesheetRuleToChildren(stylesheetRule, currentPath);

  m_text->applyStylesheetRuleWithSelector(stylesheetRule, currentPath);
}
