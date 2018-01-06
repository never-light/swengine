#pragma once

#include <Engine\Components\Graphics\RenderSystem\MeshLoader.h>

class OpenGL3MeshLoader : public MeshLoader {
public:
	Mesh* createMesh() override;
};