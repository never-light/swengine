#pragma once

#include "GameObject.h"
#include "Graphics\SolidMesh.h"

class SolidGameObject : public GameObject {
public:
	SolidGameObject(SolidMesh* mesh);
	virtual ~SolidGameObject();

	virtual void render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram) override;

	Transform* getTransform() const;

	std::vector<OBB> getColliders() const;
protected:
	Transform* m_transform;
	SolidMesh* m_mesh;

	GpuProgram* m_gpuProgram;
};