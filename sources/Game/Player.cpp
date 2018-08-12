#include "Player.h"

#include <Engine\assertions.h>

Player::Player(SolidMesh * armsMesh, BaseMaterial* baseMaterial)
	: Renderable(baseMaterial),
	m_armsMesh(armsMesh), 
	m_transform(new Transform())
{
	_assert(m_armsMesh->getColliders().size() == 1);
	_assert(m_armsMesh->hasSkeleton());
}

Player::~Player()
{
	delete m_transform;
}

void Player::render()
{
	if (m_baseMaterial->isTransformsDataRequired())
		m_baseMaterial->getGpuProgram()->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());

	m_armsMesh->render(m_baseMaterial);
}

Transform * Player::getTransform() const
{
	return m_transform;
}

OBB Player::getWorldPlacedCollider() const
{
	return OBB(m_armsMesh->getColliders()[0], m_transform->getTransformationMatrix());
}

vector3 Player::getPosition() const
{
	return m_transform->getPosition();
}

Skeleton* Player::getSkeleton() const
{
	return m_armsMesh->getSkeleton();
}

void Player::applyPose(const SkeletonPose & pose)
{
	m_armsMesh->getSkeleton()->applyPose(pose);
}
