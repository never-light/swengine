#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
	Player();
	virtual ~Player();

	virtual void render(const Camera* camera) override;

	Transform* getTransform() const;
protected:
	Transform * m_transform;
};