#pragma once

#include <string>
#include <Engine\Components\Graphics\Mesh.h>

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();

	std::vector<Mesh*> load(const std::string&);
};