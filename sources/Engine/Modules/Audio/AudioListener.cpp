#include "precompiled.h"

#pragma hdrstop

#include "AudioListener.h"

#include <utility>

#include "ALDebug.h"
#include "Modules/Math/MathUtils.h"

AudioListener::AudioListener()
{
  AL_CALL_BLOCK_BEGIN();
  setPosition({0.0f, 0.0f, 0.0f});
  setVelocity({0.0, 0.0f, 0.0f});
  setOrientation(MathUtils::IDENTITY_QUAT);
  AL_CALL_BLOCK_END();
}

AudioListener::~AudioListener()
{
}

void AudioListener::setPosition(const glm::vec3& position)
{
  alListener3f(AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 AudioListener::getPosition() const
{
  glm::vec3 position;
  alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);

  return position;
}

void AudioListener::setVelocity(const glm::vec3& velocity)
{
  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

glm::vec3 AudioListener::getVelocity() const
{
  glm::vec3 velocity;
  alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);

  return velocity;
}

void AudioListener::setOrientation(const glm::quat& orientation)
{
  auto [forward, up] = MathUtils::quatToForwardUp(orientation);
  float orientationData[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };

  alListenerfv(AL_ORIENTATION, orientationData);
}

glm::quat AudioListener::getOrientation() const
{
  float orientationData[6] = {0};
  alGetListenerfv(AL_ORIENTATION, orientationData);

  return MathUtils::forwardUpToQuat({ orientationData[0], orientationData[1], orientationData[2] },
    {orientationData[3], orientationData[4], orientationData[5]});
}
