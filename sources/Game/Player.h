#pragma once

#include "GameObject.h"
#include "Graphics\SolidMesh.h"

class Player : public GameObject {
public:
	Player(SolidMesh* armsMesh);
	virtual ~Player();

	virtual void render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram) override;
	Transform* getTransform() const;

	OBB getWorldPlacedCollider() const;
	vector3 getPosition() const;

	Skeleton* getSkeleton() const;

private:
	Transform * m_transform;
	SolidMesh* m_armsMesh;
};