#pragma once

#include <Engine/Components/Graphics/RenderLayout/Mesh.h>

class MeshComponent {
public:
	MeshComponent(Mesh* mesh);

	Mesh* getMesh() const;
	void setMesh(Mesh* mesh);

private:
	Mesh* m_mesh;
};