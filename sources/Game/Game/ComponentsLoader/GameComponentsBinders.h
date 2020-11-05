#pragma once

#include "Engine/Modules/ECS/ECS.h"
#include "Engine/Modules/LevelsManagement/GameObjectsComponentsBinders.h"
#include "Engine/Modules/ResourceManagement/ResourcesManager.h"
#include "Engine/Modules/Graphics/OpenGL/GLTexture.h"

#include "GameComponentsBindingParameters.h"


class PlayerComponentBinder : public GameObjectsComponentParametricBinder<PlayerComponentBindingParameters> {
 public:
  explicit PlayerComponentBinder(const PlayerComponentBindingParameters& componentParameters);
  PlayerComponentBinder() = default;
  ~PlayerComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;
};

class InventoryItemComponentBinder : public GameObjectsComponentParametricBinder<InventoryItemComponentBindingParameters> {
 public:
  explicit InventoryItemComponentBinder(const InventoryItemComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourceManager);
  InventoryItemComponentBinder() = default;
  ~InventoryItemComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<ResourcesManager> m_resourceManager;
};

class InventoryComponentBinder : public GameObjectsComponentParametricBinder<InventoryComponentBindingParameters> {
 public:
  explicit InventoryComponentBinder(const InventoryComponentBindingParameters& componentParameters,
    std::shared_ptr<GameWorld> gameWorld);
  InventoryComponentBinder() = default;
  ~InventoryComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
};

class InteractiveComponentBinder : public GameObjectsComponentParametricBinder<InteractiveComponentBindingParameters> {
 public:
  explicit InteractiveComponentBinder(const InteractiveComponentBindingParameters& componentParameters);
  InteractiveComponentBinder() = default;
  ~InteractiveComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;
};

class ActorComponentBinder : public GameObjectsComponentParametricBinder<ActorComponentBindingParameters> {
 public:
  explicit ActorComponentBinder(const ActorComponentBindingParameters& componentParameters);
  ActorComponentBinder() = default;
  ~ActorComponentBinder() override = default;

  void bindToObject(GameObject& gameObject) override;
};