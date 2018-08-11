#include "Animator.h"

#include <Engine\assertions.h>

Animator::Animator(Skeleton* skeleton)
	: m_currentAnimation(nullptr), 
	m_skeleton(skeleton), 
	m_currentTime(0.0f), 
	m_currentPose(skeleton->getBonesCount()),
	m_currentAnimationState(AnimationState::Stopped)
{

}

Animator::~Animator()
{
}

void Animator::setCurrentAnimation(Animation * animation)
{
	m_currentAnimation = animation;
	m_currentTime = 0.0f;
}

Animation * Animator::getCurrentAnimation()
{
	return m_currentAnimation;
}

void Animator::increaseAnimationTime(float delta)
{
	if (m_currentAnimation == nullptr || m_currentAnimationState == AnimationState::Stopped)
		return;

	m_currentTime += delta * m_currentAnimation->getSpeed() * m_currentAnimation->getSpeedFactor();

	if (m_currentTime > m_currentAnimation->getDuration()) {
		if (m_currentAnimation->getEndBehaviour() == Animation::EndBehaviour::Repeat) {
			m_currentTime -= m_currentAnimation->getDuration();
		}
		else {
			m_currentTime = 0.0f;
			m_currentAnimationState = AnimationState::Stopped;
		}
	}

	updatePose();
}

void Animator::updatePose() {
	m_currentPose.reset();

	for (const BoneAnimation& boneAnimation : m_currentAnimation->getBonesAnimations()) {
		size_t boneTransformIndex = m_skeleton->getBones()[boneAnimation.getBoneIndex()].getId();

		const BonePositionKeyFrame* nextPosition = &boneAnimation.getPositionKeyFrames()[0];
		const BonePositionKeyFrame* prevPosition = &boneAnimation.getPositionKeyFrames()[0];

		for (const BonePositionKeyFrame& keyFrame : boneAnimation.getPositionKeyFrames()) {
			nextPosition = &keyFrame;

			if (keyFrame.timeStamp > m_currentTime) {
				break;
			}

			prevPosition = &keyFrame;
		}

		const BoneOrientationKeyFrame* nextOrientation = &boneAnimation.getOrientationKeyFrames()[0];
		const BoneOrientationKeyFrame* prevOrientation = &boneAnimation.getOrientationKeyFrames()[0];

		for (const BoneOrientationKeyFrame& keyFrame : boneAnimation.getOrientationKeyFrames()) {
			nextOrientation = &keyFrame;

			if (keyFrame.timeStamp > m_currentTime) {
				break;
			}

			prevOrientation = &keyFrame;
		}

		// Position interpolation
		float positionsDelta = nextPosition->timeStamp - prevPosition->timeStamp;

		float positionsProgress;

		if (positionsDelta <= 1e-5) {
			positionsProgress = 0.0f;
		}
		else {
			positionsProgress = (m_currentTime - prevPosition->timeStamp) / positionsDelta;
		}

		vector3 interpolatedPosition = glm::mix(prevPosition->position, nextPosition->position, positionsProgress);
		
		// Orientation interpolation
		float orientationsDelta = nextOrientation->timeStamp - prevOrientation->timeStamp;
		float orientationsProgress;

		if (orientationsDelta <= 1e-5) {
			orientationsProgress = 0.0f;
		}
		else {
			orientationsProgress = (m_currentTime - prevOrientation->timeStamp) / orientationsDelta;
		}

		quaternion interpolatedOrientation = glm::slerp(prevOrientation->orientation, nextOrientation->orientation, orientationsProgress);

		matrix4 boneTransform = glm::translate(matrix4(), interpolatedPosition) * glm::toMat4(interpolatedOrientation) * glm::scale(vector3(1.0f, 1.0f, 1.0f));
		m_currentPose.setBoneTransform(boneTransformIndex, boneTransform);
	}
}

bool Animator::isPlaying() const
{
	return m_currentAnimationState == AnimationState::Playing;
}

bool Animator::isStopped() const
{
	return m_currentAnimationState == AnimationState::Stopped;
}

void Animator::play()
{
	m_currentAnimationState = AnimationState::Playing;
}

void Animator::stop()
{
	m_currentAnimationState = AnimationState::Stopped;
}

const SkeletonPose & Animator::getAnimatedPose() const
{
	return m_currentPose;
}