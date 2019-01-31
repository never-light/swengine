#include "Animation.h"

Animation::Animation(float duration, float speed, const std::vector<BoneAnimation>& keyframes)
	: m_duration(duration), 
	m_speed(speed), 
	m_keyframes(keyframes),
	m_endBehaviour(EndBehaviour::Repeat),
	m_speedFactor(1.0f)
{
}

Animation::~Animation()
{
}

const std::vector<BoneAnimation>& Animation::getBonesAnimations() const
{
	return m_keyframes;
}

float Animation::getDuration() const
{
	return m_duration;
}

void Animation::setSpeed(float speed)
{
	m_speed = speed;
}

float Animation::getSpeed() const
{
	return m_speed;
}

void Animation::setSpeedFactor(float speedFactor)
{
	m_speedFactor = speedFactor;
}

float Animation::getSpeedFactor() const
{
	return m_speedFactor;
}

void Animation::setEndBehaviour(EndBehaviour behaviour)
{
	m_endBehaviour = behaviour;
}

Animation::EndBehaviour Animation::getEndBehaviour() const
{
	return m_endBehaviour;
}
