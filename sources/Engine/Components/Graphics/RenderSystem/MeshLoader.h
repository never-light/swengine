#pragma once

#include <vector>
#include "Mesh.h"

class MeshLoader {
public:
	virtual Mesh* createMesh() = 0;
	virtual std::vector<Mesh*> loadFromFile(const std::string& filename);
};