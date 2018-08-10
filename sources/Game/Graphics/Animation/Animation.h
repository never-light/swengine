#pragma once

#include <Engine\Components\ResourceManager\Resource.h>
#include "BoneAnimation.h"

class Animation {
public:
	Animation(float duration, float speed, const std::vector<BoneAnimation>& keyframes);
	virtual ~Animation();

	const std::vector<BoneAnimation>& getBonesAnimations() const; 

	float getDuration() const;

	void setSpeed(float speed);
	float getSpeed() const;
private:
	// Duration, ticks
	float m_duration;

	// Animation speed, ticks per second
	float m_speed; 

	std::vector<BoneAnimation> m_keyframes;
};