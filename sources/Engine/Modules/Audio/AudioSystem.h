#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/GraphicsScene.h"

#include "AudioSourceComponent.h"
#include "AudioListener.h"

class AudioSystem : public GameSystem,
                    public EventsListener<GameObjectAddComponentEvent<AudioSourceComponent>> {
 public:
  explicit AudioSystem(std::shared_ptr<GraphicsScene> environmentState);
  ~AudioSystem() override;

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;

  [[nodiscard]] const AudioListener& getListener() const;
  [[nodiscard]] AudioListener& getListener();

 private:
  EventProcessStatus receiveEvent(const GameObjectAddComponentEvent<AudioSourceComponent>& event) override;

 private:
  ALCdevice* m_audioDevice{};
  ALCcontext* m_audioContext{};

  std::shared_ptr<GraphicsScene> m_environmentState;

  std::unique_ptr<AudioListener> m_audioListener;
};
