#pragma once

#include <vector>
#include <Engine/Components/AL/al.h>
#include <Engine/Components/Math/types.h>

class SoundListener
{

public:
	SoundListener(const vector3 & position, const vector3 & velocity,
		const vector3 & lookTarget, const vector3 & worldUp = vector3(0, 1, 0));
	~SoundListener();

	void setLookTarget(const vector3 & lookTarget);
	void setWorldUp(const vector3 & worldUp);
	void setPosition(const vector3 & position);
	void setVelocity(const vector3 & velocity);

	 std::vector<float>  getOrientation(const vector3 & target, const vector3 & worldUp);
	 vector3  getPosition();
	 vector3  getVelocity();

private:

	vector3 m_position;
	vector3 m_velocity;
	vector3 m_lookTarget;
	vector3 m_worldUp;

};