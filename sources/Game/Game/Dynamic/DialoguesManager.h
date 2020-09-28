#pragma once

#include <Engine/Modules/ECS/ECS.h>

#include "Dialogue.h"
#include "ActorComponent.h"

class DialogueState {
 public:
  DialogueState();
  DialogueState(std::string dialogueId, std::string lastPhraseId);

  [[nodiscard]] const std::string& getDialogueId() const;
  [[nodiscard]] const std::string& getLastPhraseId() const;

  void setDialogueId(const std::string& id);
  void setLastPhraseId(const std::string& id);

  void setInitial(bool isInitialState = true);
  [[nodiscard]] bool isInitial() const;

  [[nodiscard]] bool hasLastPhrase() const;

 private:
  std::string m_dialogueId;
  std::string m_lastPhraseId;

  bool m_isInitialState{};
};

struct DialogueResponse {
 public:
  DialogueResponse(GameObject initiator, GameObject target, std::string dialogueId, std::string phraseId);

  [[nodiscard]] GameObject getInitiator() const;
  [[nodiscard]] GameObject getTarget() const;

  [[nodiscard]] const std::string& getDialogueId() const;
  [[nodiscard]] const std::string& getPhraseId() const;

 private:
  GameObject m_initiator;
  GameObject m_target;

  std::string m_dialogueId;
  std::string m_phraseId;
};

class DialoguesManager {
 public:
  DialoguesManager() = default;

  void addDialogue(const Dialogue& dialogue);
  [[nodiscard]] const Dialogue& getDialogue(const std::string& id) const;

  [[nodiscard]] bool hasDialogue(const std::string& id) const;

  void loadFromFile(const std::string& path);

  /**
   * @brief Starts dialogue between player and NPC
   *
   * Dialogue can bed started with two states - NPC first or player first
   * (it depends on flags in actor configuration).
   *
   * To continue dialog call triggerDialoguePhrase function with required
   * response id.
   *
   * This method should be used to trigger dialogue mode by some players actions,
   * for example on pressing action button or something etc.
   *
   * @param initiator Initiator game object, should be always a player
   * @param target Target game object, should be object with Actor component
   *
   * @return Current dialogue state
   */
  [[nodiscard]] std::vector<DialogueResponse> startAnyDialogue(GameObject initiator, GameObject target,
    DialogueState& state);

  /**
   * @brief Continues dialog with specified player response.
   *
   * This method triggers player response and appropriate NPC response.
   * The previous dialog in DialogueState can be switched to another dialog after
   * method calling.
   *
   * It is assumed that DialogueState will represent last NPC phrase, so
   * continueDialogue makes sense only for player responses. After
   * triggering player response, NPC response will be triggered automatically,
   * and DialogueState will store its last phrase.
   *
   * @param phraseId Player phrase it
   * @param state Current dialogue state that should be updated
   *
   * @return Player responses list to continue the dialog
   */
  [[nodiscard]] std::vector<DialogueResponse> continueDialogue(
    const DialogueResponse& response,
    DialogueState& state);

  /**
   * @brief Stops active dialog
   *
   * @param state Current dialogue state
   */
  void stopDialogue(DialogueState& state);

 private:
  void applyPhrase(
    const Dialogue& dialogue,
    const DialoguePhrase& phrase,
    GameObject initiator,
    GameObject target);

  [[nodiscard]] bool isPhraseAvailable(
    const Dialogue& dialogue,
    const DialoguePhrase& phrase,
    GameObject phraseInitiator,
    GameObject phraseTarget);

  [[nodiscard]] std::vector<DialoguePhrase> findAvailableResponses(
    const Dialogue& dialogue,
    const DialoguePhrase& phrase,
    GameObject responseInitiator,
    GameObject responseTarget);

  [[nodiscard]] std::vector<ActorDialogue> findAvailableDialogues(
    GameObject playerObject,
    GameObject actorObject);

  [[nodiscard]] bool isDialogueAvailable(
    const Dialogue& dialogue,
    GameObject dialogueInitiator,
    GameObject dialogueTarget);

  [[nodiscard]] std::vector<ActorDialogue> findAvailablePlayerDialogues(
    GameObject playerObject,
    GameObject actorObject);

  [[nodiscard]] std::vector<DialogueResponse> buildDialogueResponsesList(
    const Dialogue& dialogue,
    const std::vector<DialoguePhrase>& phrasesList,
    GameObject playerObject,
    GameObject actorObject);

  [[nodiscard]] std::vector<DialogueResponse> buildDialogueResponsesList(
    const std::vector<ActorDialogue>& dialoguesList,
    GameObject playerObject,
    GameObject actorObject);

 private:
  std::unordered_map<std::string, Dialogue> m_dialogues;
};
