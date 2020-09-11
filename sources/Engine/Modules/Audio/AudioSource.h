#pragma once

#include <memory>
#include <list>

#include <glm/vec3.hpp>

#include "AudioClip.h"

enum class AudioSourceState {
  Playing, Paused, Stopped
};

class AudioSource {
 public:
  explicit AudioSource(std::shared_ptr<AudioClip> clip);
  AudioSource(const AudioSource& source);

  ~AudioSource();

  void setPitch(float pitch);
  [[nodiscard]] float getPitch() const;

  void setVolume(float volume);
  [[nodiscard]] float getVolume() const;

  void setClip(std::shared_ptr<AudioClip> clip);
  [[nodiscard]] std::shared_ptr<AudioClip> getClip() const;

  void setPosition(const glm::vec3& position);
  [[nodiscard]] glm::vec3 getPosition() const;

  void setVelocity(const glm::vec3& velocity);
  [[nodiscard]] glm::vec3 getVelocity() const;

  void setLooped(bool isLooped);
  [[nodiscard]] bool isLooped() const;

  void setRelativeToListenerMode(bool relativeToListener);
  [[nodiscard]] bool isRelativeToListener() const;

  void play();
  [[nodiscard]] bool isPlaying() const;

  void pause();
  [[nodiscard]] bool isPaused() const;

  void stop();
  [[nodiscard]] bool isStopped() const;

  void playOnce(std::shared_ptr<AudioClip> clip);

 private:
  [[nodiscard]] ALuint getALSource() const;

  void updateInternalState();

 private:
  ALuint m_source{};
  AudioSourceState m_sourceState{AudioSourceState::Stopped};

  std::shared_ptr<AudioClip> m_audioClip;

  std::list<AudioSource> m_subSources;

 private:
  friend class AudioSystem;
};
