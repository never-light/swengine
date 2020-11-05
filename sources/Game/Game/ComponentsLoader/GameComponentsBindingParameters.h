#pragma once

#include "Modules/LevelsManagement/GameObjectsBindingParameters.h"

class PlayerComponentBindingParameters {
 public:
  [[nodiscard]] float getHeight() const;
  void setHeight(float height);

  [[nodiscard]] float getWalkSpeed() const;
  void setWalkSpeed(float walkSpeed);

 private:
  float m_height{};
  float m_walkSpeed{};
};

class ActorComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getName() const;
  void setName(const std::string& name);

  [[nodiscard]] float getHealth() const;
  void setHealth(float health);

  [[nodiscard]] float getHealthLimit() const;
  void setHealthLimit(float healthLimit);

  [[nodiscard]] const std::vector<std::pair<std::string, bool>>& getDialoguesList() const;
  void setDialoguesList(const std::vector<std::pair<std::string, bool>>& dialoguesList);

  void addDialogue(const std::string& dialogueName, bool isStartedByNPC);

 private:
  std::string m_name;
  float m_health{};
  float m_healthLimit{};

  std::vector<std::pair<std::string, bool>> m_dialoguesList;
 };

class InventoryComponentBindingParameters {
 public:
  [[nodiscard]] const std::vector<std::string>& getItemsNames() const;
  void setItemsNames(const std::vector<std::string>& itemsNames);

  void addItemName(const std::string& itemName);

 private:
  std::vector<std::string> m_itemsNames;
};

class InteractiveComponentBindingParameters {
 public:
  [[nodiscard]] bool isTakeable() const;
  void setIsTakeable(bool isTakeable);

  [[nodiscard]] bool isTalkable() const;
  void setIsTalkable(bool isTalkable);

  [[nodiscard]] const std::string& getTitle() const;
  void setTitle(const std::string& title);

  [[nodiscard]] bool isUsable() const;
  void setIsUsable(bool isUsable);

 private:
  std::string m_title;

  bool m_isTakeable = false;
  bool m_isTalkable = false;
  bool m_isUsable = false;
};

class InventoryItemComponentBindingParameters {
 public:
  [[nodiscard]] const std::string& getShortDescription() const;
  void setShortDescription(const std::string& shortDescription);

  [[nodiscard]] const std::string& getLongDescription() const;
  void setLongDescription(const std::string& longDescription);

  [[nodiscard]] const std::string& getName() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& getTitle() const;
  void setTitle(const std::string& title);

  [[nodiscard]] const std::string& getIconResourceName() const;
  void setIconResourceName(const std::string& iconResourceName);

  [[nodiscard]] bool isReadable() const;
  void setIsReadable(bool isReadable);

  [[nodiscard]] bool isUsable() const;
  void setIsUsable(bool isUsable);

  [[nodiscard]] bool isDroppable() const;
  void setIsDroppable(bool isDroppable);

 private:
  std::string m_shortDescription;
  std::string m_longDescription;
  std::string m_name;
  std::string m_title;
  std::string m_iconResourceName;

  bool m_isReadable{};
  bool m_isUsable{};
  bool m_isDroppable{};
};
