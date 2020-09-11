#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"

#include "AudioSourceComponent.h"
#include "AudioListener.h"

class AudioSystem : public GameSystem,
                    public EventsListener<GameObjectAddComponentEvent<AudioSourceComponent>> {
 public:
  explicit AudioSystem(std::shared_ptr<SharedGraphicsState> environmentState);
  ~AudioSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

  [[nodiscard]] const AudioListener& getListener() const;
  [[nodiscard]] AudioListener& getListener();

 private:
  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectAddComponentEvent<AudioSourceComponent>& event) override;

 private:
  ALCdevice* m_audioDevice{};
  ALCcontext* m_audioContext{};

  std::shared_ptr<SharedGraphicsState> m_environmentState;

  std::unique_ptr<AudioListener> m_audioListener;
};
