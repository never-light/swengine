#pragma once

#include <string>
#include <vector>

#include <Engine\types.h>
#include "Vertex.h"
#include "HardwareBuffer.h"

class Mesh {
public:
	Mesh();
	Mesh(HardwareBuffer* geometryBuffer);
	virtual ~Mesh();

	virtual void addSubMesh(Mesh* mesh);
	virtual Mesh* getSubMesh(size_t index);
	virtual const std::vector<Mesh*>& getSubMeshesArray() const;

	virtual void setName(const std::string& name);
	virtual const std::string& getName() const;

	virtual HardwareBuffer* getGeometryBuffer() const;

protected:
	std::string m_name;
	std::vector<Mesh*> m_subMeshes;

	HardwareBuffer* m_geometryBuffer;
};