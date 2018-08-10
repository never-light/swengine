#include "Animation.h"

Animation::Animation(float duration, float speed, const std::vector<BoneAnimation>& keyframes)
	: m_duration(duration), m_speed(speed), m_keyframes(keyframes)
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
