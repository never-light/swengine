#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLGeometryStore.h"
#include "GUIWidget.h"
#include "BitmapFont.h"

class GUITextBox;

class GUIText : public GUIWidget {
 public:
  GUIText();
  GUIText(ResourceHandle<BitmapFont> font, std::string text);

  void setFont(ResourceHandle<BitmapFont> font);
  [[nodiscard]] ResourceHandle<BitmapFont> getFont() const;

  void setText(const std::string& text);
  [[nodiscard]] std::string getText() const;

  void setColor(const glm::vec4& color, GUIWidgetVisualState visualState = GUIWidgetVisualState::Default);
  [[nodiscard]] glm::vec4 getColor(GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const;

  void setFontSize(int size);
  [[nodiscard]] int getFontSize() const;

  void render(GUISystem& guiSystem) override;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

  void setSize(const glm::ivec2& size) override;
  [[nodiscard]] glm::ivec2 getSize() const override;

  void setWidth(int width) override;
  void setHeight(int height) override;

 protected:
  [[nodiscard]] glm::mat4 updateTransformationMatrix() override;

 private:
  void resetTextGeometryCache();

  std::tuple<std::vector<glm::vec3>,
             std::vector<glm::vec2>,
             std::vector<uint16_t>, glm::ivec2> getStringGeometryStoreParams(const std::string& str) const;

  Mesh* updateAndGetGeometryStore();
  Mesh* createStringGeometryBuffer(const std::string& str);

 private:
  ResourceHandle<BitmapFont> m_font;

  std::string m_text;

  int m_fontSize;

  mutable std::unique_ptr<Mesh> m_textGeometryCache;
  mutable bool m_needTextGeometryUpdate = true;

 private:
  friend class GUITextBox;
};

