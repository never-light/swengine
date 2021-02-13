#pragma once

#include <Engine/Modules/Graphics/GUI/GUILayout.h>
#include <Engine/Modules/Graphics/GUI/GUIText.h>
#include <Engine/Modules/Input/InputModule.h>

#include "InventoryComponent.h"

class InventoryUI : public GUILayout {
 public:
  using ItemActionCallback = std::function<void(GameObject)>;

 public:
  InventoryUI(std::weak_ptr<GameWorld> gameWorld,
    std::shared_ptr<InputModule> inputModule);

  void setPadding(int padding);
  [[nodiscard]] int getPadding() const;

  void setColumnsCount(int columnsCount);
  [[nodiscard]] int getColumnsCount() const;

  void setRowsCount(int rowsCount);
  [[nodiscard]] int getRowsCount() const;

  void setCellSize(const glm::ivec2& size);
  [[nodiscard]] const glm::ivec2& getCellSize() const;

  void setIconOrigin(const glm::ivec2& origin);
  [[nodiscard]] const glm::ivec2& getIconOrigin() const;

  void setIconSize(const glm::ivec2& size);
  [[nodiscard]] const glm::ivec2& getIconSize() const;

  void updateLayout();

  void setInventoryOwner(const GameObject& inventoryOwner);
  [[nodiscard]] GameObject getInventoryOwner() const;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

 protected:
  void onShow() override;

 private:
  void showTooltipWindow(const glm::ivec2& origin, GameObject itemObject);
  void hideTooltipWindow();

  void showDetailedDescWindow(GameObject itemObject);
  void hideDetailedDescWindow();

  void showContextActionMenu(const glm::ivec2& origin, GameObject itemObject);
  void hideContextActionMenu();

  void contextMenuRegisterActionWidget(const std::shared_ptr<GUIText>& widget, int offsetY);

  void syncGUIWithOwnerInventory();

 private:
  std::weak_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<InputModule> m_inputModule;

  GameObject m_inventoryOwner{};

  glm::ivec2 m_cellSize{};
  glm::ivec2 m_iconOrigin{};
  glm::ivec2 m_iconSize{};

  int m_padding{};
  int m_columnsCount{};
  int m_rowsCount{};

  std::vector<std::shared_ptr<GUILayout>> m_cells;

  std::shared_ptr<GUILayout> m_cellsLayout;

  std::shared_ptr<GUILayout> m_tooltip;
  std::shared_ptr<GUIText> m_tooltipTitle;
  std::shared_ptr<GUIText> m_tooltipDesc;

  std::shared_ptr<GUILayout> m_detailedDesc;
  std::shared_ptr<GUIText> m_detailedDescTitle;
  std::shared_ptr<GUIText> m_detailedDescText;

  std::shared_ptr<GUILayout> m_contextMenu;

  std::shared_ptr<GUIText> m_contextShowDetailedDescLink;
  std::shared_ptr<GUIText> m_contextUseLink;
  std::shared_ptr<GUIText> m_contextDropLink;
};

