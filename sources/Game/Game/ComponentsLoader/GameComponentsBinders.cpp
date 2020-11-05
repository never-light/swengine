#include "GameComponentsBinders.h"

#include "Game/PlayerComponent.h"
#include "Game/Inventory/InventoryComponent.h"
#include "Game/Dynamic/InteractiveObjectComponent.h"
#include "Game/Dynamic/ActorComponent.h"

PlayerComponentBinder::PlayerComponentBinder(const PlayerComponentBindingParameters& componentParameters)
  : GameObjectsComponentParametricBinder<PlayerComponentBindingParameters>(componentParameters)
{

}

void PlayerComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto& playerComponent = *gameObject.addComponent<PlayerComponent>(bindingParameters.getHeight()).get();
  playerComponent.setMovementSpeed(bindingParameters.getWalkSpeed());
}

InventoryItemComponentBinder::InventoryItemComponentBinder(const InventoryItemComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourceManager)
  : GameObjectsComponentParametricBinder<InventoryItemComponentBindingParameters>(componentParameters),
    m_resourceManager(std::move(resourceManager))
{

}

void InventoryItemComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto iconTexture = m_resourceManager->getResource<GLTexture>(bindingParameters.getIconResourceName());

  auto& inventoryItemComponent = *gameObject.addComponent<InventoryItemComponent>(
    iconTexture, bindingParameters.getName(), bindingParameters.getTitle()).get();

  inventoryItemComponent.setReadable(bindingParameters.isReadable());
  inventoryItemComponent.setUsable(bindingParameters.isUsable());
  inventoryItemComponent.setDroppable(bindingParameters.isDroppable());

  inventoryItemComponent.setShortDescription(bindingParameters.getShortDescription());
  inventoryItemComponent.setLongDescription(bindingParameters.getLongDescription());

}

InventoryComponentBinder::InventoryComponentBinder(const InventoryComponentBindingParameters& componentParameters,
  std::shared_ptr<GameWorld> gameWorld)
  : GameObjectsComponentParametricBinder<InventoryComponentBindingParameters>(componentParameters),
    m_gameWorld(std::move(gameWorld))
{

}

void InventoryComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  gameObject.addComponent<InventoryComponent>();

  for (const std::string& itemObjectName : bindingParameters.getItemsNames()) {
    GameObject itemObject = m_gameWorld->findGameObject(itemObjectName);

    if (!itemObject.isAlive()) {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Inventory item object {} is not alive at the loading time", itemObjectName));
    }

    m_gameWorld->emitEvent<InventoryItemActionCommandEvent>(
      {gameObject,
        InventoryItemActionTriggerType::RelocateToInventory,
        itemObject});
  }

}

InteractiveComponentBinder::InteractiveComponentBinder(const InteractiveComponentBindingParameters& componentParameters)
  : GameObjectsComponentParametricBinder<InteractiveComponentBindingParameters>(componentParameters)
{

}

void InteractiveComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto& interactiveComponent = *gameObject.addComponent<InteractiveObjectComponent>().get();

  interactiveComponent.setName(bindingParameters.getTitle());
  interactiveComponent.setTakeable(bindingParameters.isTakeable());
  interactiveComponent.setUsable(bindingParameters.isUsable());
  interactiveComponent.setTalkable(bindingParameters.isTalkable());
}

ActorComponentBinder::ActorComponentBinder(const ActorComponentBindingParameters& componentParameters)
  : GameObjectsComponentParametricBinder<ActorComponentBindingParameters>(componentParameters)
{

}

void ActorComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto& actorComponent = *gameObject.addComponent<ActorComponent>().get();

  actorComponent.setName(bindingParameters.getName());

  for (const auto&[dialogueId, isStartedByNPC] : bindingParameters.getDialoguesList()) {
    actorComponent.addDialogue(ActorDialogue(dialogueId, isStartedByNPC));
  }

  actorComponent.setHealth(bindingParameters.getHealth());
  actorComponent.setHealthLimit(bindingParameters.getHealthLimit());
}
