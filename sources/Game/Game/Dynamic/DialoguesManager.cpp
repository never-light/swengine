#include "DialoguesManager.h"

#include <utility>
#include <optional>

#include <Engine/Utility/xml.h>
#include <Engine/Utility/files.h>
#include <Engine/Utility/numbers.h>
#include <Engine/Utility/containers.h>

#include "ActorComponent.h"
#include "Game/PlayerComponent.h"

DialogueState::DialogueState(
  std::string dialogueId,
  std::string lastPhraseId)
  : m_dialogueId(std::move(dialogueId)),
    m_lastPhraseId(std::move(lastPhraseId))
{

}

const std::string& DialogueState::getDialogueId() const
{
  return m_dialogueId;
}

const std::string& DialogueState::getLastPhraseId() const
{
  return m_lastPhraseId;
}

void DialogueState::setLastPhraseId(const std::string& id)
{
  m_lastPhraseId = id;
}

void DialogueState::setDialogueId(const std::string& id)
{
  m_dialogueId = id;
}

DialogueState::DialogueState()
  : m_isInitialState(true)
{

}

void DialogueState::setInitial(bool isInitial)
{
  m_isInitialState = isInitial;
}

bool DialogueState::isInitial() const
{
  return m_isInitialState;
}

bool DialogueState::hasLastPhrase() const
{
  return !m_lastPhraseId.empty();
}

DialogueResponse::DialogueResponse(
  GameObject initiator,
  GameObject target,
  std::string dialogueId,
  std::string phraseId)
  : m_initiator(initiator),
    m_target(target),
    m_dialogueId(std::move(dialogueId)),
    m_phraseId(std::move(phraseId))
{

}

GameObject DialogueResponse::getInitiator() const
{
  return m_initiator;
}

GameObject DialogueResponse::getTarget() const
{
  return m_target;
}

const std::string& DialogueResponse::getDialogueId() const
{
  return m_dialogueId;
}

const std::string& DialogueResponse::getPhraseId() const
{
  return m_phraseId;
}

void DialoguesManager::addDialogue(const Dialogue& dialogue)
{
  m_dialogues.insert({dialogue.getId(), dialogue});
}

const Dialogue& DialoguesManager::getDialogue(const std::string& id) const
{
  return m_dialogues.at(id);
}

bool DialoguesManager::hasDialogue(const std::string& id) const
{
  return m_dialogues.contains(id);
}

void DialoguesManager::loadFromFile(const std::string& path)
{
  std::string dialogueDescriptionPath = FileUtils::getGameResourcePath("dialogues/" + path + ".xml");

  spdlog::debug("Load dialogues list from {}", dialogueDescriptionPath);

  auto dialogueDescription = XMLUtils::openDescriptionFile(dialogueDescriptionPath, "dialogues");

  pugi::xml_node dialoguesListNode = std::get<1>(dialogueDescription);

  for (pugi::xml_node dialogueNode : dialoguesListNode.children("dialogue")) {
    std::string dialogueId = dialogueNode.attribute("id").as_string();

    Dialogue dialogue(dialogueId);

    for (pugi::xml_node phraseNode : dialogueNode.children("phrase")) {
      std::string phraseId = phraseNode.attribute("id").as_string();
      std::string phraseContent = phraseNode.child_value("content");

      DialoguePhrase phrase(phraseId, phraseContent);

      for (pugi::xml_node responseNode : phraseNode.children("response")) {
        std::string responseId = responseNode.attribute("id").as_string();

        phrase.addResponse(responseId);
      }

      dialogue.addPhrase(phrase);
    }

    // TODO: add dialogue validation here

    addDialogue(dialogue);
  }

}

std::vector<DialogueResponse> DialoguesManager::startAnyDialogue(
  GameObject initiator,
  GameObject target,
  DialogueState& state)
{
  // NOTE: assume that initiator is always a player
  SW_ASSERT(initiator.hasComponent<PlayerComponent>());
  SW_ASSERT(initiator.hasComponent<ActorComponent>() && target.hasComponent<ActorComponent>());

  auto targetActorComponent = initiator.getComponent<ActorComponent>();

  auto availableDialogues = findAvailableDialogues(initiator, target);

  state = DialogueState();

  if (availableDialogues.empty()) {
    // If there is not available dialogues, return initial state,
    // so a caller should decide how to handle it.

    state.setInitial();

    return {};
  }

  ActorDialogue startDialogue = availableDialogues[0];

  // Try to find any dialogues with isStartedByNPC flag to start from first of them

  for (auto& actorDialogue : availableDialogues) {
    if (actorDialogue.isStartedByNPC()) {
      startDialogue = actorDialogue;
    }
  }

  if (startDialogue.isStartedByNPC()) {
    const Dialogue& dialogue = getDialogue(startDialogue.getDialogueId());

    // Start the dialog with NPC as initiator
    GameObject dialogueInitiator = target;
    GameObject dialogueTarget = initiator;

    // Apply NCP first phrase
    applyPhrase(dialogue, dialogue.getStartPhrase(), dialogueInitiator, dialogueTarget);

    state = DialogueState(dialogue.getId(), dialogue.getStartPhrase().getId());

    // Find player responses
    auto playerResponses = findAvailableResponses(dialogue, dialogue.getStartPhrase(),
      dialogueInitiator, dialogueTarget);

    if (playerResponses.empty()) {
      state.setInitial();

      return buildDialogueResponsesList(findAvailablePlayerDialogues(initiator, target),
        initiator, target);
    }
    else {
      return buildDialogueResponsesList(dialogue, playerResponses, initiator, target);
    }
  }
  else {
    // Player will be first, but dialog is not known here,
    // so go into initial state.
    state.setInitial();

    return buildDialogueResponsesList(findAvailablePlayerDialogues(initiator, target),
      initiator, target);
  }

}

std::vector<DialogueResponse> DialoguesManager::continueDialogue(
  const DialogueResponse& response,
  DialogueState& state)
{
  // Check parameters consistency
  SW_ASSERT(response.getDialogueId() == state.getDialogueId() ||
    state.isInitial());

  const Dialogue* activeDialogue = nullptr;
  const DialoguePhrase* playerResponsePhrase = nullptr;

  if (state.isInitial()) {
    // Start new dialog in which the player will be an initiator

    activeDialogue = &getDialogue(response.getDialogueId());
    playerResponsePhrase = &activeDialogue->getStartPhrase();

    SW_ASSERT(response.getPhraseId() == playerResponsePhrase->getId());

    state.setDialogueId(activeDialogue->getId());
  }
  else {
    SW_ASSERT(state.getDialogueId() == response.getDialogueId());

    activeDialogue = &getDialogue(response.getDialogueId());
    playerResponsePhrase = &activeDialogue->getPhrase(response.getPhraseId());
  }

  // Apply player response
  applyPhrase(*activeDialogue, *playerResponsePhrase, response.getInitiator(), response.getTarget());

  // Find actor responses
  auto actorResponses = findAvailableResponses(*activeDialogue, *playerResponsePhrase,
    response.getTarget(), response.getInitiator());

  if (actorResponses.empty()) {
    // If there is not any actor responses, stop the dialog and go into initial state

    state.setLastPhraseId("");
    state.setInitial();

    return buildDialogueResponsesList(findAvailablePlayerDialogues(response.getInitiator(), response.getTarget()),
      response.getInitiator(), response.getTarget());
  }

  // Apply random NPC response

  DialoguePhrase actorResponse = Random::get(actorResponses);

  applyPhrase(*activeDialogue, *playerResponsePhrase, response.getTarget(), response.getInitiator());

  state.setLastPhraseId(actorResponse.getId());

  // Find player responses
  auto playerResponses = findAvailableResponses(*activeDialogue, actorResponse,
    response.getInitiator(), response.getTarget());

  if (playerResponses.empty()) {
    // If there is not any player responses, stop the dialog and go into initial state

    state.setInitial();

    return buildDialogueResponsesList(findAvailablePlayerDialogues(response.getInitiator(), response.getTarget()),
      response.getInitiator(), response.getTarget());
  }

  std::vector<DialogueResponse> playerDialogueResponses;
  playerDialogueResponses.reserve(playerResponses.size());

  for (const auto& responsePhrase : playerResponses) {
    playerDialogueResponses.emplace_back(response.getInitiator(), response.getTarget(),
      activeDialogue->getId(), responsePhrase.getId());
  }

  state.setInitial(false);

  return playerDialogueResponses;
}

void DialoguesManager::stopDialogue(DialogueState& state)
{
  ARG_UNUSED(state);

  // NOTE: this method does nothing now and is used for
  // consistency.
}

void DialoguesManager::applyPhrase(
  const Dialogue& dialogue,
  const DialoguePhrase& phrase,
  GameObject initiator,
  GameObject target)
{
  ARG_UNUSED(dialogue);
  ARG_UNUSED(phrase);
  ARG_UNUSED(initiator);
  ARG_UNUSED(target);

  // TODO: add actions triggering here
}

std::vector<DialoguePhrase> DialoguesManager::findAvailableResponses(
  const Dialogue& dialogue,
  const DialoguePhrase& phrase,
  GameObject responseInitiator,
  GameObject responseTarget)
{
  ARG_UNUSED(responseInitiator);
  ARG_UNUSED(responseTarget);

  std::vector<DialoguePhrase> responses;

  for (const auto& responseId : phrase.getResponses()) {
    const DialoguePhrase& response = dialogue.getPhrase(responseId);

    if (isPhraseAvailable(dialogue, response, responseInitiator, responseTarget)) {
      responses.push_back(response);
    }
  }

  return responses;
}

bool DialoguesManager::isPhraseAvailable(
  const Dialogue& dialogue,
  const DialoguePhrase& phrase,
  GameObject phraseInitiator,
  GameObject phraseTarget)
{
  ARG_UNUSED(dialogue);
  ARG_UNUSED(phrase);
  ARG_UNUSED(phraseInitiator);
  ARG_UNUSED(phraseTarget);

  // TODO: check preconditions here

  return true;
}

std::vector<ActorDialogue> DialoguesManager::findAvailableDialogues(
  GameObject playerObject,
  GameObject actorObject)
{
  std::vector<ActorDialogue> availableDialogues;

  const auto& actorDialogues = actorObject.getComponent<ActorComponent>()->getDialogues();

  for (auto& actorDialogue : actorDialogues) {
    const Dialogue& dialogue = getDialogue(actorDialogue.getDialogueId());

    if (actorDialogue.isStartedByNPC()) {
      if (isDialogueAvailable(dialogue, actorObject, playerObject)) {
        availableDialogues.push_back(actorDialogue);
      }
    }
    else {
      if (isDialogueAvailable(dialogue, playerObject, actorObject)) {
        availableDialogues.push_back(actorDialogue);
      }
    }
  }

  return availableDialogues;
}

bool DialoguesManager::isDialogueAvailable(
  const Dialogue& dialogue,
  GameObject dialogueInitiator,
  GameObject dialogueTarget)
{
  ARG_UNUSED(dialogue);
  ARG_UNUSED(dialogueInitiator);
  ARG_UNUSED(dialogueTarget);

  return isPhraseAvailable(dialogue, dialogue.getStartPhrase(), dialogueInitiator, dialogueTarget);
}

std::vector<ActorDialogue> DialoguesManager::findAvailablePlayerDialogues(
  GameObject playerObject,
  GameObject actorObject)
{
  return ContainersUtils::filter(
    findAvailableDialogues(playerObject, actorObject), [](const ActorDialogue& dialogue) {
      return !dialogue.isStartedByNPC();
    });
}

std::vector<DialogueResponse> DialoguesManager::buildDialogueResponsesList(
  const Dialogue& dialogue,
  const std::vector<DialoguePhrase>& phrasesList,
  GameObject playerObject,
  GameObject actorObject)
{
  std::vector<DialogueResponse> responses;
  responses.reserve(phrasesList.size());

  for (const auto& phrase : phrasesList) {
    responses.emplace_back(playerObject, actorObject, dialogue.getId(), phrase.getId());
  }

  return responses;
}

std::vector<DialogueResponse> DialoguesManager::buildDialogueResponsesList(
  const std::vector<ActorDialogue>& dialoguesList,
  GameObject playerObject,
  GameObject actorObject)
{
  std::vector<DialogueResponse> responses;
  responses.reserve(dialoguesList.size());

  for (const auto& dialogue : dialoguesList) {
    std::string startPhraseId = getDialogue(dialogue.getDialogueId()).getStartPhrase().getId();

    responses.emplace_back(playerObject, actorObject,
      dialogue.getDialogueId(), startPhraseId);
  }

  return responses;
}
