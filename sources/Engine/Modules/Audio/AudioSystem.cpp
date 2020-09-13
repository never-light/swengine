#include "precompiled.h"

#pragma hdrstop

#include "AudioSystem.h"

#include <utility>
#include <spdlog/spdlog.h>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Exceptions/exceptions.h"
#include "ALDebug.h"

AudioSystem::AudioSystem(std::shared_ptr<SharedGraphicsState> environmentState)
  : m_environmentState(std::move(environmentState))
{

}

AudioSystem::~AudioSystem()
{
  SW_ASSERT(m_audioListener == nullptr);
  SW_ASSERT(m_audioContext == nullptr);
  SW_ASSERT(m_audioDevice == nullptr);
}

void AudioSystem::configure(GameWorld* gameWorld)
{
  spdlog::info("Configure audio system");

  AL_CALL_BLOCK_BEGIN();
  m_audioDevice = alcOpenDevice(nullptr);
  AL_CALL_BLOCK_END();

  if (m_audioDevice == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, "Audio device can not be opened");
  }

  AL_CALL_BLOCK_BEGIN();
  m_audioContext = alcCreateContext(m_audioDevice, nullptr);
  AL_CALL_BLOCK_END();

  if (m_audioContext == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, "Audio context can not be created");
  }

  AL_CALL_BLOCK_BEGIN();
  bool contextMakingCurrentStatus = alcMakeContextCurrent(m_audioContext);
  AL_CALL_BLOCK_END();

  if (!contextMakingCurrentStatus) {
    THROW_EXCEPTION(EngineRuntimeException, "Audio context can not be set as current");
  }

  m_audioListener = std::make_unique<AudioListener>();

  gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<AudioSourceComponent>>(this);
}

void AudioSystem::unconfigure(GameWorld* gameWorld)
{
  spdlog::info("Unconfigure audio system");

  gameWorld->unsubscribeEventsListener<GameObjectAddComponentEvent<AudioSourceComponent>>(this);

  m_audioListener.reset();

  alcMakeContextCurrent(nullptr);

  alcDestroyContext(m_audioContext);
  m_audioContext = nullptr;

  alcCloseDevice(m_audioDevice);
  m_audioDevice = nullptr;
}

void AudioSystem::update(GameWorld* gameWorld, float delta)
{
  ARG_UNUSED(delta);

  const Transform& currentCameraTransform = *m_environmentState->getActiveCamera()->getTransform();

  m_audioListener->setPosition(currentCameraTransform.getPosition());
  m_audioListener->setOrientation(currentCameraTransform.getOrientation());

  for (auto object : gameWorld->allWith<AudioSourceComponent>()) {
    auto transformComponent = object.getComponent<TransformComponent>();
    auto& audioSource = object.getComponent<AudioSourceComponent>()->getSource();

    audioSource.updateInternalState();

    if (!transformComponent->isStatic()) {
      audioSource.setPosition(transformComponent->getTransform().getPosition());
    }
  }
}

const AudioListener& AudioSystem::getListener() const
{
  return *m_audioListener;
}

AudioListener& AudioSystem::getListener()
{
  return *m_audioListener;
}

EventProcessStatus AudioSystem::receiveEvent(GameWorld* gameWorld,
  const GameObjectAddComponentEvent<AudioSourceComponent>& event)
{
  ARG_UNUSED(gameWorld);

  auto& transform = event.gameObject.getComponent<TransformComponent>()->getTransform();
  auto source = event.component->getSourcePtr();

  source->setPosition(transform.getPosition());

  return EventProcessStatus::Processed;
}
