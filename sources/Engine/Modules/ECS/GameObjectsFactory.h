#include <utility>

#pragma once

class GameObject;

class BaseGameObjectsComponentBinder {
 public:
  BaseGameObjectsComponentBinder() = default;
  virtual ~BaseGameObjectsComponentBinder() = default;

  virtual void bindToObject(GameObject& gameObject) = 0;
};

template<class ComponentType>
class GameObjectsComponentBinder : public BaseGameObjectsComponentBinder {
 public:
  using ComponentBindingParameters = typename ComponentType::BindingParameters;

 public:
  GameObjectsComponentBinder() = default;
  ~GameObjectsComponentBinder() override = default;
};

class BaseGameObjectsComponentsBindersFactory {
 public:
  BaseGameObjectsComponentsBindersFactory() = default;
  virtual ~BaseGameObjectsComponentsBindersFactory() = default;
};

template<class ComponentType>
class GameObjectsComponentsBindersFactory : public BaseGameObjectsComponentsBindersFactory {
 public:
  GameObjectsComponentsBindersFactory() = default;
  ~GameObjectsComponentsBindersFactory() override = default;

  virtual std::unique_ptr<GameObjectsComponentBinder<ComponentType>>
  createBinder(const typename ComponentType::BindingParameters& bindingParameters) = 0;
};

class ResourcesManager;
class GameWorld;

enum class GameObjectsComponentsBinderInjectParameters {
  None,
  ResourcesManager,
  GameWorld
};

template<class ComponentType, class BinderType,
  GameObjectsComponentsBinderInjectParameters injectContext = GameObjectsComponentsBinderInjectParameters::None>
class GameObjectsComponentsGenericBindersFactory : public GameObjectsComponentsBindersFactory<ComponentType> {
 public:
  GameObjectsComponentsGenericBindersFactory() = default;

  explicit GameObjectsComponentsGenericBindersFactory(std::shared_ptr<ResourcesManager> resourcesManager)
    : m_resourcesManager(std::move(resourcesManager))
  {

  };
  explicit GameObjectsComponentsGenericBindersFactory(std::shared_ptr<GameWorld> gameWorld)
    : m_gameWorld(std::move(gameWorld))
  {

  };

  ~GameObjectsComponentsGenericBindersFactory() override = default;

  std::unique_ptr<GameObjectsComponentBinder<ComponentType>>
  createBinder(const typename ComponentType::BindingParameters& bindingParameters) override
  {
    if constexpr (injectContext == GameObjectsComponentsBinderInjectParameters::ResourcesManager) {
      return std::make_unique<BinderType>(bindingParameters, m_resourcesManager);
    }
    else if constexpr (injectContext == GameObjectsComponentsBinderInjectParameters::GameWorld) {
      return std::make_unique<BinderType>(bindingParameters, m_gameWorld);
    }
    else {
      return std::make_unique<BinderType>(bindingParameters);
    }
  }

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
  std::shared_ptr<GameWorld> m_gameWorld;
};
