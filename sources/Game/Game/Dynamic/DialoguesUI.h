#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/GUI/GUILayout.h>

#include "DialoguesManager.h"
#include "QuestsSystem.h"

struct DialoguesUIHistoryItem {
 public:
  DialoguesUIHistoryItem(std::string initiatorName, std::string phraseText);

  [[nodiscard]] const std::string& getInitiatorName() const;
  [[nodiscard]] const std::string& getPhraseText() const;

 private:
  std::string m_initiatorName;
  std::string m_phraseText;
};

class DialoguesUI : public GUILayout,
                    public EventsListener<QuestStartedEvent>,
                    public EventsListener<QuestCompletedEvent>,
                    public EventsListener<QuestFailedEvent>,
                    public EventsListener<QuestTaskStartedEvent>,
                    public EventsListener<QuestTaskCompletedEvent>,
                    public EventsListener<QuestTaskFailedEvent>,
                    public EventsListener<InventoryItemTransferEvent> {
 public:
  DialoguesUI(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<DialoguesManager> dialoguesManager,
    std::shared_ptr<QuestsStorage> questsStorage);

  void setPhrasesMargin(const glm::ivec2& margin);
  [[nodiscard]] const glm::ivec2& getPhrasesMargin() const;

  void startDialogue(GameObject initiator, GameObject target);
  void stopDialogue();

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;

  void updatePendingResponses();

  std::shared_ptr<QuestsStorage> getQuestsStorage() const;

 public:
  EventProcessStatus receiveEvent(const QuestStartedEvent& event) override;
  EventProcessStatus receiveEvent(const QuestCompletedEvent& event) override;
  EventProcessStatus receiveEvent(const QuestFailedEvent& event) override;
  EventProcessStatus receiveEvent(const QuestTaskStartedEvent& event) override;
  EventProcessStatus receiveEvent(const QuestTaskCompletedEvent& event) override;
  EventProcessStatus receiveEvent(const QuestTaskFailedEvent& event) override;
  EventProcessStatus receiveEvent(const InventoryItemTransferEvent& event) override;

 protected:
  void onShow() override;
  void onHide() override;

 private:
  void triggerResponsePhrase(const DialogueResponse& response);

  void updateUILayout(const std::vector<DialogueResponse>& dialogueResponses);

  void updateResponsesLayout(const std::vector<DialogueResponse>& dialogueResponses);
  void addPhrase(GameObject initiator, GameObject target, const DialoguePhrase& phrase);
  void addNotification(const std::string& notification);
  void addTextMessage(const std::string& message);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<DialoguesManager> m_dialoguesManager;
  std::shared_ptr<QuestsStorage> m_questsStorage;

  GameObject m_initiator{};
  GameObject m_target{};

  DialogueState m_dialogueState{};

  std::shared_ptr<GUILayout> m_phrasesLayout;
  std::shared_ptr<GUILayout> m_responsesLayout;

  std::vector<DialoguesUIHistoryItem> m_dialogueHistory;

  std::optional<DialogueResponse> m_pendingResponse;

  glm::ivec2 m_phrasesMargin{};
};
