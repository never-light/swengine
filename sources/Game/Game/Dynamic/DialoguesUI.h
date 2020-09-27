#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/GUI/GUILayout.h>

#include "Game/Dynamic/DialoguesManager.h"

class DialoguesUI : public GUILayout {
 public:
  DialoguesUI(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<DialoguesManager> dialoguesManager);

  void startDialog(GameObject initiator, GameObject target);
  void stopDialog();

 private:
  void triggerResponsePhrase(const DialogueResponse& response);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<DialoguesManager> m_dialoguesManager;

  GameObject m_initiator{};
  GameObject m_target{};

  DialogueState m_dialogueState{};

  std::shared_ptr<GUILayout> m_phrasesLayout;
  std::shared_ptr<GUILayout> m_responsesLayout;
};
