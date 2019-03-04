#include "SoundListener.h"


SoundListener::SoundListener(const vector3 & position, const vector3 & velocity,
	const vector3 & lookTarget, const vector3 & worldUp) :
	m_position(position), m_velocity(velocity), m_lookTarget(lookTarget), m_worldUp(worldUp)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	alListenerfv(AL_ORIENTATION, getOrientation(lookTarget, worldUp).data());
}

SoundListener::~SoundListener()
{
}

 std::vector<float> SoundListener::getOrientation(const vector3 & target, const vector3 & worldUp)
{
	return std::vector<float> { target.x, target.y, target.z, worldUp.x, worldUp.y, worldUp.z };
}

void SoundListener::setLookTarget(const vector3 & lookTarget)
{
	this->m_lookTarget = lookTarget;
	alListenerfv(AL_ORIENTATION, getOrientation(lookTarget, this->m_worldUp).data());
}

void SoundListener::setWorldUp(const vector3 & worldUp)
{
	this->m_worldUp = worldUp;
	alListenerfv(AL_ORIENTATION, getOrientation(this->m_lookTarget, worldUp).data());
}

void SoundListener::setPosition(const vector3 & position)
{
	this -> m_position = position;
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void SoundListener::setVelocity(const vector3 & velocity)
{
	this->m_velocity = velocity;
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

 vector3  SoundListener::getPosition()
{
	return this->m_position;
}

 vector3  SoundListener::getVelocity()
{
	return this->m_velocity;
}
