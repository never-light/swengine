#pragma once

#include <string>
#include <vector>

#include <Engine/Modules/ECS/ECS.h>
#include "InfoportionsSystem.h"

/**
 * @brief Command to initiate dialogue between player (initiator)
 * and specified NPC (target).
 *
 * This command intended to be handled in player controlling system.
 */
struct ActorTalkTriggerCommandEvent {
 public:
  ActorTalkTriggerCommandEvent(GameObject initiator,
    GameObject target)
    : initiator(initiator),
      target(target)
  {

  }

 public:
  GameObject initiator;
  GameObject target;
};

struct ActorDialogue {
 public:
  ActorDialogue(std::string dialogueId, bool isStartedByNCP);

  [[nodiscard]] const std::string& getDialogueId() const;
  [[nodiscard]] bool isStartedByNPC() const;

 private:
  std::string m_dialogueId;
  bool m_isStartedByNCP{};
};

struct ActorComponent {
 public:
  ActorComponent() = default;

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void addDialogue(const ActorDialogue& dialogue);
  [[nodiscard]] const std::vector<ActorDialogue>& getDialogues() const;

  [[nodiscard]] bool hasDialogue(const std::string& dialogueId) const;

  [[nodiscard]] ActorInfoportionsStorage& getInfoportionsStorage();
  [[nodiscard]] const ActorInfoportionsStorage& getInfoportionsStorage() const;

 private:
  std::string m_name;
  std::vector<ActorDialogue> m_dialogues;
  ActorInfoportionsStorage m_infoportionsStorage;
};
