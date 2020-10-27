#pragma once

#include <memory>
#include <string>
#include <functional>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>
#include <Engine/Modules/Graphics/OpenGL/GLTexture.h>

struct InventoryItemComponent {
 public:
  // Owner, item
  using ActionCallback = std::function<void(GameObject, GameObject)>;

 public:
  InventoryItemComponent(
    ResourceHandle<GLTexture> icon,
    std::string id,
    std::string name);

  void setIcon(ResourceHandle<GLTexture> icon);
  [[nodiscard]] ResourceHandle<GLTexture> getIcon() const;

  void setId(const std::string& id);
  [[nodiscard]] const std::string& getId() const;

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

  template<class T>
  inline void addComponentToRestore()
  {
    m_componentsToRestoreMask.set(ComponentsTypeInfo::getTypeIndex<T>());
  }

  template<class T>
  inline void removeComponentToRestore()
  {
    m_componentsToRestoreMask.reset(ComponentsTypeInfo::getTypeIndex<T>());
  }

  template<class T>
  [[nodiscard]] inline bool shouldComponentBeRestored() const
  {
    return m_componentsToRestoreMask.test(ComponentsTypeInfo::getTypeIndex<T>());
  }

  /*!
   * @brief Sets the use callback.
   *
   * @details Use callback is called on item use
   *
   * @param callback
   */
  void setUseCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getUseCallback() const;

  /*!
   * @brief Sets the drop callback
   *
   * The drop callback is called after some object is removed
   * from inventory.
   *
   * @param callback
   */
  void setDropCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getDropCallback() const;

  /*!
   * @brief Sets the take callback
   *
   * The task callback is called after some object is added
   * to inventory.
   *
   * @param callback
   */
  void setTakeCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getTakeCallback() const;

  /*!
   * @brief Sets the read callback
   *
   * The read callback is called after some inventory owner
   * reads the item description (from UI only).
   *
   * @param callback
   */
  void setReadCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getReadCallback() const;

 private:
  ResourceHandle<GLTexture> m_icon;
  std::string m_id;
  std::string m_name;
  std::string m_shortDescription;
  std::string m_longDescription;

  bool m_isReadable{};
  bool m_isUsable{};
  bool m_isDroppable{};

  ActionCallback m_useCallback;
  ActionCallback m_dropCallback;
  ActionCallback m_takeCallback;
  ActionCallback m_readCallback;

  GameObject m_owner;

  std::bitset<GameObjectData::MAX_COMPONENTS_COUNT> m_componentsToRestoreMask;
};

