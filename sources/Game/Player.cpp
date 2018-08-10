#include "Player.h"

#include <Engine\assertions.h>

Player::Player(SolidMesh * armsMesh)
	: m_armsMesh(armsMesh), 
	m_transform(new Transform())
{
	_assert(m_armsMesh->getColliders().size() == 1);
	_assert(m_armsMesh->hasSkeleton());
}

Player::~Player()
{
	delete m_transform;
}

void Player::render(GraphicsContext* graphicsContext, GpuProgram * gpuProgram)
{
	gpuProgram->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());
	m_armsMesh->render(graphicsContext, gpuProgram);
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
