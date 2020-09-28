#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/GUI/GUILayout.h>

#include "Game/Dynamic/DialoguesManager.h"

struct DialoguesUIHistoryItem {
 public:
  DialoguesUIHistoryItem(std::string initiatorName, std::string phraseText);

  [[nodiscard]] const std::string& getInitiatorName() const;
  [[nodiscard]] const std::string& getPhraseText() const;


 private:
  std::string m_initiatorName;
  std::string m_phraseText;
};

class DialoguesUI : public GUILayout {
 public:
  DialoguesUI(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<DialoguesManager> dialoguesManager);

  void setPhrasesMargin(const glm::ivec2& margin);
  [[nodiscard]] const glm::ivec2& getPhrasesMargin() const;

  void startDialogue(GameObject initiator, GameObject target);
  void stopDialogue();

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

  void updatePendingResponses();

 protected:
  void onShow() override;
  void onHide() override;

 private:
  void triggerResponsePhrase(const DialogueResponse& response);

  void updateUILayout(const std::vector<DialogueResponse>& dialogueResponses);

  void updateResponsesLayout(const std::vector<DialogueResponse>& dialogueResponses);
  void addPhrase(GameObject initiator, GameObject target, const DialoguePhrase& phrase);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<DialoguesManager> m_dialoguesManager;

  GameObject m_initiator{};
  GameObject m_target{};

  DialogueState m_dialogueState{};

  std::shared_ptr<GUILayout> m_phrasesLayout;
  std::shared_ptr<GUILayout> m_responsesLayout;

  std::vector<DialoguesUIHistoryItem> m_dialogueHistory;

  std::optional<DialogueResponse> m_pendingResponse;

  glm::ivec2 m_phrasesMargin{};
};
