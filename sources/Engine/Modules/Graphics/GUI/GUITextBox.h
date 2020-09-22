#pragma once

#include <string>
#include <memory>

#include "GUIWidget.h"
#include "BitmapFont.h"
#include "GUIText.h"

class GUITextBox : public GUIWidgetRect {
 public:
  explicit GUITextBox(std::shared_ptr<BitmapFont> font);

  void setFont(std::shared_ptr<BitmapFont> font);
  [[nodiscard]] std::shared_ptr<BitmapFont> getFont() const;

  void setText(const std::string& text);
  [[nodiscard]] std::string getText() const;

  void setTextColor(const glm::vec4& color, GUIWidgetVisualState visualState = GUIWidgetVisualState::Default);
  [[nodiscard]] glm::vec4 getTextColor(GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const;

  void setTextFontSize(int size);
  [[nodiscard]] int getTextFontSize() const;

  void render(GUISystem& guiSystem) override;

  [[nodiscard]] bool canHaveFocus() const override;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

  void applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule,
    const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath) override;

 protected:
  void processKeyboardEvent(const GUIKeyboardEvent& event) override;

  [[nodiscard]] glm::mat4 updateTransformationMatrix() override;

 private:
  std::shared_ptr<BitmapFont> m_font;
  std::shared_ptr<GUIText> m_text;
};
