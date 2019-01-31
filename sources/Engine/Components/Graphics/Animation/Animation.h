#pragma once

#include "BoneAnimation.h"

class Animation {
public:
	enum EndBehaviour {
		Stop, Repeat
	};

public:
	Animation(float duration, float speed, const std::vector<BoneAnimation>& keyframes);
	virtual ~Animation();

	const std::vector<BoneAnimation>& getBonesAnimations() const; 

	float getDuration() const;

	void setSpeed(float speed);
	float getSpeed() const;

	void setSpeedFactor(float speedFactor);
	float getSpeedFactor() const;

	void setEndBehaviour(EndBehaviour behaviour);
	EndBehaviour getEndBehaviour() const;

private:
	// Duration, ticks
	float m_duration;

	// Animation speed, ticks per second
	float m_speed; 

	// Animation speed multiplier
	float m_speedFactor;

	std::vector<BoneAnimation> m_keyframes;

	EndBehaviour m_endBehaviour;
};