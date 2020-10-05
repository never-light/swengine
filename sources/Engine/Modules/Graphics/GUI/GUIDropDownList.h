#pragma once

#include <string>
#include <memory>

#include "GUIWidget.h"
#include "BitmapFont.h"
#include "GUIText.h"
#include "GUILayout.h"

struct GUIDropDownListItem {
  int16_t id;
  std::string value;

  std::shared_ptr<GUIText> textWidget;
};

class GUIDropDownList : public GUIWidgetRect {
 public:
  void addItem(const std::string& text, const std::string& value);

  void setCurrentItemById(int16_t id);
  [[nodiscard]] const std::string& getCurrentItemValue() const;

  void setItemsMargin(const glm::ivec2& margin);
  [[nodiscard]] const glm::ivec2& getItemsMargin() const;

  [[nodiscard]] bool canHaveFocus() const override;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

  void applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule,
    const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath) override;

  void render(GUISystem& guiSystem) override;

 public:
  static std::shared_ptr<GUIDropDownList> create();

 protected:
  void processKeyboardEvent(const GUIKeyboardEvent& event) override;
  void processMouseButtonEvent(const GUIMouseButtonEvent& event) override;

  void onLostFocus() override;

 private:
  void switchToFoldedMode();
  void switchToUnfoldedMode();

  const GUIDropDownListItem* getItemById(int16_t id) const;
  GUIDropDownListItem* getItemById(int16_t id);

 private:
  GUIDropDownList();

 private:
  void updateLayout();

 private:
  std::shared_ptr<GUILayout> m_dropDownLayout;
  std::vector<GUIDropDownListItem> m_items;

  int16_t m_currentItemId = -1;
  int16_t m_freeItemId = 0;

  bool m_isItemsLayoutOutdated{};
  std::optional<glm::ivec2> m_preservedFoldedSize{};

  glm::ivec2 m_itemsMargin{};
};
