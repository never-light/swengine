#pragma once

#include <string>
#include <vector>

#include <Engine\types.h>
#include "Vertex.h"

class Mesh {
public:
	virtual void addSubMesh(Mesh* mesh) = 0;
	virtual Mesh* getSubMesh(size_t index) = 0;
	virtual const std::vector<Mesh*>& getSubMeshesArray() const = 0;

	virtual void setName(const std::string& name) = 0;
	virtual const std::string& getName() const = 0;

	virtual void addVertex(const Vertex&) = 0;
	virtual void addIndex(uint32) = 0;

	virtual size_t getVerticesCount() const = 0;
	virtual size_t getIndicesCount() const = 0;

	virtual void updateState() = 0;
};