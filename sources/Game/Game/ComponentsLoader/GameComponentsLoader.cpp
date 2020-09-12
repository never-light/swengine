#include "GameComponentsLoader.h"

#include "Game/PlayerComponent.h"

void GameComponentsLoader::loadPlayerData(GameObject& gameObject,
  const pugi::xml_node& data)
{
  float height = data.attribute("height").as_float(1.0f);

  auto& playerComponent = gameObject.addComponent<PlayerComponent>(height);

  float walk_speed = data.attribute("walk_speed").as_float(1.0f);
  playerComponent.setMovementSpeed(walk_speed);
}
