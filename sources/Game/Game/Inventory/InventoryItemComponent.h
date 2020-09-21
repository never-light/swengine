#pragma once

#include <memory>
#include <string>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/OpenGL/GLTexture.h>

struct InventoryItemComponent {
 public:
  InventoryItemComponent(
    std::shared_ptr<GLTexture> icon,
    std::string name);

  void setIcon(std::shared_ptr<GLTexture> icon);
  [[nodiscard]] std::shared_ptr<GLTexture> getIcon() const;

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void setShortDescription(const std::string& description);
  [[nodiscard]] const std::string& getShortDescription() const;

  void setLongDescription(const std::string& description);
  [[nodiscard]] const std::string& getLongDescription() const;

  void setReadable(bool isReadable);
  [[nodiscard]] bool isReadable() const;

  void setUsable(bool isUsable);
  [[nodiscard]] bool isUsable() const;

  void setDroppable(bool isDroppable);
  [[nodiscard]] bool isDroppable() const;

  void setOwner(const GameObject& owner);
  [[nodiscard]] const GameObject& getOwner() const;

 private:
  std::shared_ptr<GLTexture> m_icon;
  std::string m_name;
  std::string m_shortDescription;
  std::string m_longDescription;

  bool m_isReadable{};
  bool m_isUsable{};
  bool m_isDroppable{};

  GameObject m_owner;
};

