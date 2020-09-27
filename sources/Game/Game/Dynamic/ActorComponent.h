#pragma once

#include <string>
#include <vector>

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

 private:
  std::string m_name;
  std::vector<ActorDialogue> m_dialogues;
};
