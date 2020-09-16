#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Utility/xml.h>

class GameComponentsLoader {
 public:
  GameComponentsLoader() = default;

  void loadPlayerData(GameObject& gameObject, const pugi::xml_node& data);
};
