#pragma once

#include "GameObject.h"
#include <Game\Graphics\SolidMesh.h>
#include <Game\Graphics\Renderable.h>

class SolidGameObject : public GameObject, public Renderable {
public:
	SolidGameObject(SolidMesh* mesh, BaseMaterial* baseMaterial);
	virtual ~SolidGameObject();

	virtual void render() override;

	Transform* getTransform() const;

	std::vector<OBB> getColliders() const;

	vector3 getPosition() const override;
protected:
	Transform* m_transform;
	SolidMesh* m_mesh;

	GpuProgram* m_gpuProgram;
};