#include "precompiled.h"

#pragma hdrstop

#include "GUIDropDownList.h"

#include <utility>
#include "GUISystem.h"

GUIDropDownList::GUIDropDownList()
  : GUIWidgetRect("drop_down_list"),
    m_dropDownLayout(std::make_shared<GUILayout>())
{

}

bool GUIDropDownList::canHaveFocus() const
{
  return true;
}

void GUIDropDownList::processKeyboardEvent(const GUIKeyboardEvent& event)
{
  ARG_UNUSED(event);
}

std::shared_ptr<GUIDropDownList> GUIDropDownList::create()
{
  auto dropDownList = std::shared_ptr<GUIDropDownList>(new GUIDropDownList());
  dropDownList->addChildWidget(dropDownList->m_dropDownLayout);

  return dropDownList;
}

void GUIDropDownList::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule);
  m_dropDownLayout->applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "items-margin") {
      SW_ASSERT(std::holds_alternative<glm::ivec2>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default &&
        "Items margin is supported only for default state");
      this->setItemsMargin(std::get<glm::ivec2>(property.getValue()));
    }
    else if (propertyName == "background") {
      // Do nothing as property should be already processed
    }
    else {
      SW_ASSERT(false);
    }
  });

  // TODO: add resize action handler and do it there
  m_dropDownLayout->setSize(getSize());
}

void GUIDropDownList::applyStylesheetRuleToChildren(
  const GUIWidgetStylesheetRule& stylesheetRule,
  const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath)
{
  GUIWidget::applyStylesheetRuleToChildren(stylesheetRule, currentPath);

  m_dropDownLayout->applyStylesheetRuleWithSelector(stylesheetRule, currentPath);
}

const std::string& GUIDropDownList::getCurrentItemValue() const
{
  SW_ASSERT(m_currentItemId != -1);

  return getItemById(m_currentItemId)->value;
}

void GUIDropDownList::addItem(const std::string& text, const std::string& value)
{
  auto textWidget = std::make_shared<GUIText>();
  textWidget->setText(text);

  int16_t itemId = m_freeItemId++;

  textWidget->setMouseButtonCallback([this, itemId](const GUIMouseButtonEvent& event) {
    if (event.type == MouseButtonEventType::ButtonDown) {
      setCurrentItemById(itemId);
    }
  });

  m_items.push_back(GUIDropDownListItem{.id = itemId, .value = value, .textWidget = textWidget});

  m_dropDownLayout->addChildWidget(textWidget);
  m_currentItemId = 0;

  m_isItemsLayoutOutdated = true;
}

void GUIDropDownList::render(GUISystem& guiSystem)
{
  if (m_isItemsLayoutOutdated) {
    updateLayout();
  }

  GUIWidgetRect::render(guiSystem);
}

void GUIDropDownList::updateLayout()
{
  m_isItemsLayoutOutdated = false;

  glm::ivec2 itemOrigin = m_itemsMargin;

  for (auto& itemData : m_dropDownLayout->getChildrenWidgets()) {
    itemData->setOrigin(itemOrigin);
    itemData->hide();

    itemOrigin.y += getSize().y;
  }

  if (!m_items.empty()) {
    auto firstWidget = *m_dropDownLayout->getChildrenWidgets().begin();
    firstWidget->show();
  }
}

void GUIDropDownList::setItemsMargin(const glm::ivec2& margin)
{
  m_itemsMargin = margin;
  m_isItemsLayoutOutdated = true;
}

const glm::ivec2& GUIDropDownList::getItemsMargin() const
{
  return m_itemsMargin;
}

void GUIDropDownList::processMouseButtonEvent(const GUIMouseButtonEvent& event)
{
  if (event.type == MouseButtonEventType::ButtonDown) {
    if (m_preservedFoldedSize.has_value()) {
      switchToFoldedMode();
    }
    else {
      switchToUnfoldedMode();
    }
  }
}

void GUIDropDownList::onLostFocus()
{
  // TODO: debug and fix set/lost focus behaviour
//  if (m_preservedFoldedSize.has_value()) {
//    switchToFoldedMode();
//  }
}

void GUIDropDownList::switchToFoldedMode()
{
  setSize(m_preservedFoldedSize.value());
  m_dropDownLayout->setSize(getSize());
  m_preservedFoldedSize.reset();

  for (bool firstItem = true; auto& textWidget : m_dropDownLayout->getChildrenWidgets()) {
    if (firstItem) {
      textWidget->show();
      firstItem = false;
    }
    else {
      textWidget->hide();
    }
  }

  setZIndex(getZIndex() - 1);
}

void GUIDropDownList::switchToUnfoldedMode()
{
  m_preservedFoldedSize = getSize();
  setSize({getSize().x, getSize().y * m_items.size()});
  m_dropDownLayout->setSize(getSize());

  for (auto& itemData : m_items) {
    itemData.textWidget->show();
  }

  setZIndex(getZIndex() + 1);
}

const GUIDropDownListItem* GUIDropDownList::getItemById(int16_t id) const
{
  for (const auto& item : m_items) {
    if (item.id == id) {
      return &item;
    }
  }

  return nullptr;
}

GUIDropDownListItem* GUIDropDownList::getItemById(int16_t id)
{
  for (auto& item : m_items) {
    if (item.id == id) {
      return &item;
    }
  }

  return nullptr;

}

void GUIDropDownList::setCurrentItemById(int16_t id)
{
  if (id != m_currentItemId) {
    auto* newCurrentItem = &m_items[id];

    m_dropDownLayout->removeChildren();
    m_dropDownLayout->addChildWidget(newCurrentItem->textWidget);
    newCurrentItem->textWidget->show();

    for (const auto& item : m_items) {
      if (item.id != newCurrentItem->id) {
        m_dropDownLayout->addChildWidget(item.textWidget);
        item.textWidget->hide();
      }
    }

    m_currentItemId = id;
    m_isItemsLayoutOutdated = true;
  }
}
