#include "Animator.h"

#include <Engine\assertions.h>

Animator::Animator(Skeleton* skeleton)
	: m_currentAnimation(nullptr), m_skeleton(skeleton), m_behaviour(Behaviour::Stop),
	m_currentTime(0.0f), m_speed(1.0f)
{
	_assert(skeleton != nullptr);
}

Animator::~Animator()
{
}

float Animator::getSpeed() const
{
	return m_speed;
}

void Animator::setSpeed(float speed)
{
	m_speed = speed;
}

void Animator::setCurrentAnimation(Animation * animation)
{
	m_currentAnimation = animation;
	m_speed = animation->getSpeed();
}

Animation * Animator::getCurrentAnimation()
{
	return m_currentAnimation;
}

void Animator::setBehaviour(Behaviour behaviour)
{
	m_behaviour = behaviour;
}

Animator::Behaviour Animator::getBehaviour() const
{
	return m_behaviour;
}

void Animator::update(float delta)
{
	m_currentTime += delta * m_speed;

	if (m_currentTime > m_currentAnimation->getDuration())
		m_currentTime -= m_currentAnimation->getDuration();

	const std::vector<BoneAnimation>& bonesAnimations = m_currentAnimation->getBonesAnimations();

	size_t bonesCount = m_skeleton->getBonesCount();
	std::vector<matrix4> bonesTransforms(bonesCount);
	std::vector<bool> marks(bonesCount, false);

	//for (size_t boneIndex = 0; boneIndex < bonesAnimations.size(); boneIndex++) {
		//const BoneAnimation& boneAnimation = bonesAnimations[boneIndex];
	for (const BoneAnimation& boneAnimation : bonesAnimations) {
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

		bonesTransforms[boneTransformIndex] = glm::translate(matrix4(), interpolatedPosition) * glm::toMat4(interpolatedOrientation) * glm::scale(vector3(1.0f, 1.0f, 1.0f));
		marks[boneTransformIndex] = true;
	}

	applyTransformsToBonesHierarchy(bonesTransforms, marks, m_skeleton->getRootBone(), matrix4());
}

bool Animator::isPlaying() const
{
	return m_currentAnimationState == AnimationState::Playing;
}

bool Animator::isStopped() const
{
	return m_currentAnimationState == AnimationState::Stopped;
}

void Animator::resetAnimation()
{
	m_skeleton->resetPose();
}

void Animator::applyTransformsToBonesHierarchy(const std::vector<matrix4>& transforms, const std::vector<bool>& marks, Bone* bone, const matrix4& parentTransform) {
	matrix4 currentBoneTransform;

	if (!marks[bone->getId()]) {
		currentBoneTransform = parentTransform * bone->getRelativeToParentSpaceTransform();
	}
	else {
		currentBoneTransform = parentTransform * transforms[bone->getId()];
	}

	for (Bone* childBone : m_skeleton->getChildBones(bone->getId())) {
		applyTransformsToBonesHierarchy(transforms, marks, childBone, currentBoneTransform);
	}

	if (marks[bone->getId()])
		//bone->setCurrentPoseTransform(currentBoneTransform * bone->getLocalToBoneSpaceTransform());
		bone->setCurrentPoseTransform(m_skeleton->getGlobalInverseTransform() * currentBoneTransform * bone->getLocalToBoneSpaceTransform());
}   