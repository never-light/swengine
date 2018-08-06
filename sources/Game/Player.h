#pragma once

#include "GameObject.h"
#include "Graphics\SolidMesh.h"

class Player : public GameObject {
public:
	Player(SolidMesh* hands);
	virtual ~Player();

	virtual void render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram) override;
	Transform* getTransform() const;

	OBB getWorldPlacedCollider() const;
	vector3 getPosition() const;
private:
	Transform * m_transform;
	SolidMesh* m_hands;
};