#pragma once

#include "Animation.h"
#include "Skeleton.h"

class Animator {
public:
	enum class AnimationState {
		Playing, Stopped
	};

public:
	Animator(Skeleton* skeleton);
	~Animator();

	void setCurrentAnimation(Animation* animation);
	Animation* getCurrentAnimation();

	void increaseAnimationTime(float delta);

	bool isPlaying() const;
	bool isStopped() const;

	void play();
	void stop();

	const SkeletonPose& getAnimatedPose() const;

private:
	void updatePose();

private:
	Animation* m_currentAnimation;
	Skeleton* m_skeleton;

	// Current animation time, between 0 and duration
	float m_currentTime;

	AnimationState m_currentAnimationState;

	SkeletonPose m_currentPose;
};