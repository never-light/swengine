#pragma once

#include "Mesh.h"
#include "Material.h"
class Model;

class SubModel {
public:
	SubModel(Model*);
	~SubModel();

	Model* getParent() const;

	void setMesh(Mesh*);
	Mesh* getMesh() const;

	void setMaterial(Material*);
	Material* getMaterial() const;

	bool hasMaterial() const;
	bool hasMesh() const;
private:
	Model* m_parent;
	Material* m_material;
	Mesh* m_mesh;
};