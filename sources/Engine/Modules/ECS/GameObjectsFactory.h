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
class GameObjectsComponentsBindersFactory {
 public:
  GameObjectsComponentsBindersFactory() = default;
  virtual ~GameObjectsComponentsBindersFactory() = default;

  std::unique_ptr<GameObjectsComponentBinder<ComponentType>>
  virtual createBinder(const typename ComponentType::BindingParameters& bindingParameters) = 0;
};