#pragma once

#include <memory>
#include <string>
#include <functional>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>
#include <Engine/Modules/Graphics/OpenGL/GLTexture.h>

struct InventoryItemComponentBindingParameters {
  std::string shortDescription;
  std::string longDescription;
  std::string name;
  std::string title;
  std::string iconResourceName;

  bool isReadable{};
  bool isUsable{};
  bool isDroppable{};

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("short_desc", shortDescription),
      cereal::make_nvp("long_desc", longDescription),
      cereal::make_nvp("name", name),
      cereal::make_nvp("title", title),
      cereal::make_nvp("icon_resource", iconResourceName),
      cereal::make_nvp("is_readable", isReadable),
      cereal::make_nvp("is_usable", isUsable),
      cereal::make_nvp("is_droppable", isDroppable));
  };
};

struct InventoryItemComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = InventoryItemComponentBindingParameters;

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

  [[nodiscard]] BindingParameters getBindingParameters() const;

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

class InventoryItemComponentBinder : public GameObjectsComponentBinder<InventoryItemComponent> {
 public:
  explicit InventoryItemComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourceManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourceManager;
};
