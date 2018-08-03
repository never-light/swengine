#pragma once

#include "GameObject.h"
#include "Graphics\SolidMesh.h"

class Player : public GameObject {
public:
	Player(SolidMesh* hands);
	virtual ~Player();

	virtual void render(GpuProgram* gpuProgram) override;
	Transform* getTransform() const;

private:
	Transform * m_transform;
	SolidMesh* m_hands;
};