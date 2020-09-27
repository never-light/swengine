#include "ActorComponent.h"

ActorDialogue::ActorDialogue(std::string dialogueId, bool isStartedByNCP)
  : m_dialogueId(dialogueId),
    m_isStartedByNCP(isStartedByNCP)
{

}

void ActorComponent::setName(const std::string& name)
{
  m_name = name;
}

const std::string& ActorComponent::getName() const
{
  return m_name;
}

const std::vector<ActorDialogue>& ActorComponent::getDialogues() const
{
  return m_dialogues;
}

bool ActorComponent::hasDialogue(const std::string& dialogueId) const
{
  for (const auto& actorDialogue : m_dialogues) {
    if (actorDialogue.getDialogueId() == dialogueId) {
      return true;
    }
  }

  return false;
}

void ActorComponent::addDialogue(const ActorDialogue& dialogue)
{
  m_dialogues.push_back(dialogue);
}

const std::string& ActorDialogue::getDialogueId() const
{
  return m_dialogueId;
}

bool ActorDialogue::isStartedByNPC() const
{
  return m_isStartedByNCP;
}
