#pragma once

#include <string>
#include <Engine\Components\Graphics\RenderSystem\Mesh.h>

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();

	std::vector<Mesh*> load(const std::string&);
};