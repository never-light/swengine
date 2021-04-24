#pragma once

#include <cereal/types/vector.hpp>
#include <Engine/Modules/ECS/ECS.h>
#include "InventoryItemComponent.h"

enum class InventoryItemActionTriggerType {
  Use,
  RelocateToInventory,
  DropFromInventory,
  //Show
};

struct InventoryItemActionEvent {
 public:
  InventoryItemActionEvent(GameObject inventoryOwner,
    InventoryItemActionTriggerType triggerType,
    GameObject item)
    : inventoryOwner(inventoryOwner),
      triggerType(triggerType),
      item(item)
  {

  }

 public:
  GameObject inventoryOwner;
  InventoryItemActionTriggerType triggerType;

  GameObject item;

 public:
  static const auto IS_LOGGING_ALLOWED = true;

};

struct InventoryItemActionCommandEvent : public InventoryItemActionEvent {
 public:
  InventoryItemActionCommandEvent(GameObject inventoryOwner,
    InventoryItemActionTriggerType triggerType,
    GameObject item)
    : InventoryItemActionEvent(inventoryOwner, triggerType, item)
  {

  }

 public:
  static const auto IS_LOGGING_ALLOWED = true;

};

struct InventoryItemTransferEvent {
 public:
  InventoryItemTransferEvent(
    GameObject initiator,
    GameObject target,
    GameObject item)
    : initiator(initiator),
      target(target),
      item(item)
  {

  }

 public:
  GameObject initiator;
  GameObject target;
  GameObject item;

 public:
  static const auto IS_LOGGING_ALLOWED = true;

};

struct InventoryItemTransferCommandEvent : public InventoryItemTransferEvent {
 public:
  InventoryItemTransferCommandEvent(
    GameObject initiator,
    GameObject target,
    GameObject item)
    : InventoryItemTransferEvent(initiator, target, item)
  {

  }

 public:
  static const auto IS_LOGGING_ALLOWED = true;

};

struct InventoryComponentBindingParameters {
 public:
  std::vector<std::string> itemsNames;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("items", itemsNames));
  };
};

class InventoryComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = InventoryComponentBindingParameters;

 public:
  InventoryComponent() = default;

  void addItem(GameObject item);
  void removeItem(GameObject item);

  [[nodiscard]] const std::vector<GameObject>& getItems() const;

  [[nodiscard]] bool hasItem(const std::string& itemId) const;
  [[nodiscard]] GameObject getItem(const std::string& itemId) const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  std::vector<GameObject> m_inventoryItems;
};

class InventoryComponentBinder : public GameObjectsComponentBinder<InventoryComponent> {
 public:
  explicit InventoryComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<GameWorld> gameWorld);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<GameWorld> m_gameWorld;
};
