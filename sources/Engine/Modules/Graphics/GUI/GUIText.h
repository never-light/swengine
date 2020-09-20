#pragma once

#include <string>
#include <memory>

#include "Modules/Graphics/OpenGL/GLGeometryStore.h"
#include "GUIWidget.h"
#include "BitmapFont.h"

class GUITextBox;

class GUIText : public GUIWidget {
 public:
  GUIText(std::shared_ptr<BitmapFont> font, std::string  text);

  void setFont(std::shared_ptr<BitmapFont> font);
  [[nodiscard]] std::shared_ptr<BitmapFont> getFont() const;

  void setText(const std::string& text);
  [[nodiscard]] std::string getText() const;

  void setColor(const glm::vec4& color, GUIWidgetVisualState visualState = GUIWidgetVisualState::Default);
  [[nodiscard]] glm::vec4 getColor(GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const;

  void setFontSize(int size);
  [[nodiscard]] int getFontSize() const;

  void render(GUISystem& guiSystem) override;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex) override;

 protected:
  [[nodiscard]] glm::mat4 updateTransformationMatrix() override;

 private:
  void resetTextGeometryCache();

  GLGeometryStore* updateAndGetGeometryStore();
  GLGeometryStore* createStringGeometryBuffer(const std::string& str);

 private:
  std::shared_ptr<BitmapFont> m_font;

  std::string m_text;

  int m_fontSize;

  mutable std::unique_ptr<GLGeometryStore> m_textGeometryCache;
  mutable bool m_needTextGeometryUpdate = true;

 private:
  friend class GUITextBox;
};

