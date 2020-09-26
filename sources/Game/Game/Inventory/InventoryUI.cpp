#include "InventoryUI.h"

#include <Engine/Modules/Graphics/GUI/GUIImage.h>
#include <Engine/Modules/Graphics/GUI/GUIText.h>

#include <utility>

InventoryUI::InventoryUI(
  std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<InputModule> inputModule)
  : GUILayout("inventory_ui"),
    m_gameWorld(std::move(gameWorld)),
    m_inputModule(std::move(inputModule))
{

}

void InventoryUI::setPadding(int padding)
{
  m_padding = padding;
}

int InventoryUI::getPadding() const
{
  return m_padding;
}

void InventoryUI::setColumnsCount(int columnsCount)
{
  m_columnsCount = columnsCount;
}

int InventoryUI::getColumnsCount() const
{
  return m_columnsCount;
}

void InventoryUI::setRowsCount(int rowsCount)
{
  m_rowsCount = rowsCount;
}

int InventoryUI::getRowsCount() const
{
  return m_rowsCount;
}

void InventoryUI::updateLayout()
{
  m_cellsLayout = std::dynamic_pointer_cast<GUILayout>(
    findChildByName("game_ui_inventory_layout_cells"));

  SW_ASSERT(
    m_cellsLayout != nullptr &&
      "Specify cells layout in the inventory UI description");

  for (const auto& cell : m_cells) {
    m_cellsLayout->removeChildWidget(cell);
  }

  m_cells.clear();

  m_tooltip = std::dynamic_pointer_cast<GUILayout>(
    findChildByName("game_ui_inventory_layout_tooltip"));

  SW_ASSERT(
    m_tooltip != nullptr &&
      "Specify tooltip layout in the inventory UI description");

  m_tooltipTitle = std::dynamic_pointer_cast<GUIText>(
    m_tooltip->findChildByName("game_ui_inventory_layout_tooltip_title"));

  m_tooltipDesc = std::dynamic_pointer_cast<GUIText>(
    m_tooltip->findChildByName("game_ui_inventory_layout_tooltip_desc"));

  SW_ASSERT(
    m_tooltipTitle != nullptr &&
      m_tooltipDesc != nullptr &&
      "Specify tooltip layout in the inventory UI description");

  m_detailedDesc = std::dynamic_pointer_cast<GUILayout>(
    findChildByName("game_ui_inventory_layout_detailed_desc"));

  SW_ASSERT(
    m_detailedDesc != nullptr &&
      "Specify detailed description layout in the inventory UI description");

  m_detailedDescTitle = std::dynamic_pointer_cast<GUIText>(
    m_detailedDesc->findChildByName("game_ui_inventory_layout_detailed_desc_title"));

  m_detailedDescText = std::dynamic_pointer_cast<GUIText>(
    m_detailedDesc->findChildByName("game_ui_inventory_layout_detailed_desc_text"));

  SW_ASSERT(
    m_detailedDescTitle != nullptr &&
      m_detailedDescText != nullptr &&
      "Specify tooltip layout in the inventory UI description");

  m_contextMenu = std::dynamic_pointer_cast<GUILayout>(
    findChildByName("game_ui_inventory_layout_context_menu"));

  SW_ASSERT(
    m_contextMenu != nullptr &&
      "Specify context menu layout in the inventory UI description");

  m_contextShowDetailedDescLink = std::dynamic_pointer_cast<GUIText>(
    m_contextMenu->findChildByName("game_ui_inventory_layout_context_menu_show_desc"));
  m_contextUseLink = std::dynamic_pointer_cast<GUIText>(
    m_contextMenu->findChildByName("game_ui_inventory_layout_context_menu_use"));
  m_contextDropLink = std::dynamic_pointer_cast<GUIText>(
    m_contextMenu->findChildByName("game_ui_inventory_layout_context_menu_drop"));

  SW_ASSERT(
    m_contextShowDetailedDescLink != nullptr &&
      m_contextUseLink != nullptr &&
      m_contextDropLink != nullptr &&
      "Specify context menu layout in the inventory UI description");

  m_contextMenu->setMouseLeaveCallback([this](const GUIMouseLeaveEvent& event) {
    ARG_UNUSED(event);
    hideContextActionMenu();
  });

  // Cells

  int minWidth = m_padding * 2 + (m_columnsCount - 1) * 2 * m_padding + m_columnsCount * m_cellSize.x;
  int minHeight = m_padding * 2 + (m_rowsCount - 1) * 2 * m_padding + m_rowsCount * m_cellSize.y;

  SW_ASSERT(getSize().x >= minWidth && getSize().y >= minHeight
    && "Inventory window size should be big enough to contain specified cells number");

  for (size_t rowIndex = 0; rowIndex < m_rowsCount; rowIndex++) {
    for (size_t colIndex = 0; colIndex < m_columnsCount; colIndex++) {
      glm::vec2 cellOrigin = {
        m_padding + colIndex * 2 * m_padding + colIndex * m_cellSize.x,
        m_padding + rowIndex * 2 * m_padding + rowIndex * m_cellSize.y,
      };

      auto cellWidget = std::make_shared<GUILayout>();

      cellWidget->setOrigin(cellOrigin);
      cellWidget->setSize(m_cellSize);

      m_cells.push_back(cellWidget);
      m_cellsLayout->addChildWidget(cellWidget);
    }
  }
}

void InventoryUI::syncGUIWithOwnerInventory()
{
  auto& inventory = *m_inventoryOwner.getComponent<InventoryComponent>().get();

  size_t cellIndex = 0;

  for (auto item : inventory.getItems()) {
    auto& itemComponent = *item.getComponent<InventoryItemComponent>().get();
    auto cellWidget = m_cells[cellIndex];

    std::shared_ptr<GUIImage> icon = std::make_shared<GUIImage>(itemComponent.getIcon());
    icon->setOrigin(m_iconOrigin);
    icon->setSize(m_iconSize);

    cellWidget->removeChildren();
    cellWidget->addChildWidget(icon);

    cellWidget->setMouseEnterCallback([this, item, cellWidget, icon](const GUIMouseEnterEvent& event) {
      ARG_UNUSED(event);

      if (!m_contextMenu->isShown() && !m_detailedDesc->isShown()) {
        showTooltipWindow(m_cellsLayout->getOrigin() +
          cellWidget->getOrigin() + icon->getOrigin() + icon->getSize() / 2, item);
      }
    });

    cellWidget->setMouseLeaveCallback([this](const GUIMouseLeaveEvent& event) {
      ARG_UNUSED(event);
      hideTooltipWindow();
    });

    cellWidget->setMouseButtonCallback([this, item, cellWidget, icon](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown) {
        if (event.button == SDL_BUTTON_RIGHT) {
          hideTooltipWindow();

          if (!m_detailedDesc->isShown()) {
            showContextActionMenu(m_cellsLayout->getOrigin() +
              cellWidget->getOrigin() + icon->getOrigin() + icon->getSize() / 2, item);
          }
        }
      }
    });

    cellIndex++;
  }

  for (size_t emptyCellIndex = cellIndex; emptyCellIndex < m_cells.size(); emptyCellIndex++) {
    auto cellWidget = m_cells[cellIndex];
    m_cells[emptyCellIndex]->removeChildren();

    cellWidget->setMouseEnterCallback(nullptr);
    cellWidget->setMouseLeaveCallback(nullptr);
    cellWidget->setMouseButtonCallback(nullptr);
  }

  setMouseButtonCallback([this](const GUIMouseButtonEvent& event) {
    if (event.type == MouseButtonEventType::ButtonDown) {
      if (m_detailedDesc->isShown()) {
        MousePosition mousePosition = m_inputModule->getMousePosition();

        if (!m_detailedDesc->isPointInside({ mousePosition.x, mousePosition.y })) {
          hideDetailedDescWindow();
        }
      }
    }
  });
}

void InventoryUI::setCellSize(const glm::ivec2& size)
{
  m_cellSize = size;
}

const glm::ivec2& InventoryUI::getCellSize() const
{
  return m_cellSize;
}

void InventoryUI::setIconOrigin(const glm::ivec2& origin)
{
  m_iconOrigin = origin;
}

const glm::ivec2& InventoryUI::getIconOrigin() const
{
  return m_iconOrigin;
}

void InventoryUI::setIconSize(const glm::ivec2& size)
{
  m_iconSize = size;
}

const glm::ivec2& InventoryUI::getIconSize() const
{
  return m_iconSize;
}

void InventoryUI::showTooltipWindow(const glm::ivec2& origin, GameObject itemObject)
{
  m_tooltipTitle->setText(itemObject.getComponent<InventoryItemComponent>()->getName());
  m_tooltipDesc->setText(itemObject.getComponent<InventoryItemComponent>()->getShortDescription());

  glm::ivec2 tooltipSize = m_tooltip->getSize();
  tooltipSize.x = glm::max(m_tooltipTitle->getSize().x, m_tooltipDesc->getSize().x) + m_padding * 2;

  m_tooltip->setSize(tooltipSize);

  m_tooltip->setOrigin(origin);
  m_tooltip->show();
}

void InventoryUI::hideTooltipWindow()
{
  m_tooltip->hide();
}

void InventoryUI::showDetailedDescWindow(GameObject itemObject)
{
  m_detailedDescTitle->setText(itemObject.getComponent<InventoryItemComponent>()->getName());
  m_detailedDescText->setText(itemObject.getComponent<InventoryItemComponent>()->getLongDescription());

  m_detailedDesc->show();
}

void InventoryUI::hideDetailedDescWindow()
{
  m_detailedDesc->hide();
}

void InventoryUI::showContextActionMenu(const glm::ivec2& origin, GameObject itemObject)
{
  m_contextMenu->removeChildren();

  auto& itemData = *itemObject.getComponent<InventoryItemComponent>().get();

  int m_contextMenuOffsetVertical = 0;

  if (itemData.isReadable()) {
    contextMenuRegisterActionWidget(m_contextShowDetailedDescLink, m_contextMenuOffsetVertical);
    m_contextMenuOffsetVertical += m_contextShowDetailedDescLink->getSize().y;

    m_contextShowDetailedDescLink->setMouseButtonCallback([this, itemObject](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {

        const auto& readCallback = GameObject(itemObject).getComponent<InventoryItemComponent>()->getReadCallback();

        if (readCallback) {
          readCallback(m_inventoryOwner, itemObject);
        }

        hideContextActionMenu();
        showDetailedDescWindow(itemObject);
      }
    });
  }

  if (itemData.isUsable()) {
    contextMenuRegisterActionWidget(m_contextUseLink, m_contextMenuOffsetVertical);
    m_contextMenuOffsetVertical += m_contextUseLink->getSize().y;

    m_contextUseLink->setMouseButtonCallback([this, itemObject](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        m_gameWorld->emitEvent<InventoryItemActionTriggeredEvent>(
          {m_inventoryOwner,
            InventoryItemActionTriggerType::Use,
            itemObject});

        hideContextActionMenu();
        syncGUIWithOwnerInventory();
      }
    });
  }

  if (itemData.isDroppable()) {
    contextMenuRegisterActionWidget(m_contextDropLink, m_contextMenuOffsetVertical);
    m_contextMenuOffsetVertical += m_contextDropLink->getSize().y;

    m_contextDropLink->setMouseButtonCallback([this, itemObject](const GUIMouseButtonEvent& event) {
      if (event.type == MouseButtonEventType::ButtonDown && event.button == SDL_BUTTON_LEFT) {
        m_gameWorld->emitEvent<InventoryItemActionTriggeredEvent>(
          {m_inventoryOwner,
            InventoryItemActionTriggerType::DropFromInventory,
            itemObject});

        hideContextActionMenu();
        syncGUIWithOwnerInventory();
      }
    });
  }

  m_contextMenu->setOrigin(origin);
  m_contextMenu->show();
}

void InventoryUI::hideContextActionMenu()
{
  m_contextMenu->hide();
}

void InventoryUI::contextMenuRegisterActionWidget(const std::shared_ptr<GUIText>& widget, int offsetY)
{
  glm::ivec2 origin = widget->getOrigin();
  origin.y = offsetY;

  widget->setOrigin(origin);
  m_contextMenu->addChildWidget(widget);
}

void InventoryUI::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUILayout::applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "padding") {
      SW_ASSERT(std::holds_alternative<int>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Padding is supported only for default state");
      this->setPadding(std::get<int>(property.getValue()));
    }
    else if (propertyName == "rows-count") {
      SW_ASSERT(std::holds_alternative<int>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Rows-count is supported only for default state");
      this->setRowsCount(std::get<int>(property.getValue()));
    }
    else if (propertyName == "columns-count") {
      SW_ASSERT(std::holds_alternative<int>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Columns-count is supported only for default state");
      this->setColumnsCount(std::get<int>(property.getValue()));
    }
    else if (propertyName == "cell-size") {
      SW_ASSERT(std::holds_alternative<glm::ivec2>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Cell-size is supported only for default state");
      this->setCellSize(std::get<glm::ivec2>(property.getValue()));
    }
    else if (propertyName == "icon-origin-offset") {
      SW_ASSERT(std::holds_alternative<glm::ivec2>(property.getValue()));
      SW_ASSERT(
        visualState == GUIWidgetVisualState::Default && "Icon-origin-offset is supported only for default state");
      this->setIconOrigin(std::get<glm::ivec2>(property.getValue()));
    }
    else if (propertyName == "icon-size") {
      SW_ASSERT(std::holds_alternative<glm::ivec2>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Icon-size is supported only for default state");
      this->setIconSize(std::get<glm::ivec2>(property.getValue()));
    }
    else if (propertyName == "background") {
      // Do nothing as property should be already processed by GUILayout
    }
    else {
      SW_ASSERT(false);
    }

  });

  if (m_rowsCount > 0 && m_columnsCount > 0) {
    updateLayout();
  }
}

void InventoryUI::onShow()
{
  m_tooltip->hide();
  m_detailedDesc->hide();
  m_contextMenu->hide();

  syncGUIWithOwnerInventory();
}

void InventoryUI::setInventoryOwner(const GameObject& inventoryOwner)
{
  SW_ASSERT(inventoryOwner.hasComponent<InventoryComponent>());
  m_inventoryOwner = inventoryOwner;

  syncGUIWithOwnerInventory();
}

GameObject InventoryUI::getInventoryOwner() const
{
  return m_inventoryOwner;
}
