#pragma once

#include "Animation.h"
#include "Skeleton.h"

class Animator {
public:
	enum class Behaviour {
		Stop, Repeat
	};

	enum class AnimationState {
		Playing, Stopped
	};

public:
	Animator(Skeleton* skeleton);
	~Animator();

	float getSpeed() const;
	void setSpeed(float speed);

	void setCurrentAnimation(Animation* animation);
	Animation* getCurrentAnimation();

	void setBehaviour(Behaviour behaviour);
	Behaviour getBehaviour() const;

	void update(float delta);

	bool isPlaying() const;
	bool isStopped() const;

private:
	void resetAnimation();
	void applyTransformsToBonesHierarchy(const std::vector<matrix4>& transforms, const std::vector<bool>& marks, Bone* bone, const matrix4& parentTransform);
private:
	Animation* m_currentAnimation;
	Skeleton* m_skeleton;

	// Speed of the animation, ticks per second
	float m_speed;

	// Current animation time, between 0 and duration
	float m_currentTime;

	Behaviour m_behaviour;
	AnimationState m_currentAnimationState;
};