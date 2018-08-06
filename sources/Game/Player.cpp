#include "Player.h"

Player::Player(SolidMesh * hands)
	: m_hands(hands), m_transform(new Transform())
{
	m_transform->setPosition(0, 0, 0);
	m_transform->setOrientation(quaternion());
	m_transform->setScale(1, 1, 1);
}

Player::~Player()
{
	delete m_transform;
}

void Player::render(GraphicsContext* graphicsContext, GpuProgram * gpuProgram)
{
	gpuProgram->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());
	m_hands->render(graphicsContext, gpuProgram);
}

Transform * Player::getTransform() const
{
	return m_transform;
}

OBB Player::getWorldPlacedCollider() const
{
	return OBB(m_hands->getColliders()[0], m_transform->getTransformationMatrix());
}

vector3 Player::getPosition() const
{
	return m_transform->getPosition();
}
