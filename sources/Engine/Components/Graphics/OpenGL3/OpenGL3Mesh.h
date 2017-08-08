#pragma once

#include <string>
#include <vector>
#include <Engine\types.h>
#include <Engine\Components\Graphics\Vertex.h>

#include "OpenGL3.h"

class OpenGL3Mesh {
public:
	OpenGL3Mesh();
	OpenGL3Mesh(const std::vector<Vertex>&);
	OpenGL3Mesh(const std::vector<Vertex>&, const std::vector<uint32>&);
	virtual ~OpenGL3Mesh();

	void addSubMesh(OpenGL3Mesh* mesh);
	OpenGL3Mesh* getSubMesh(size_t index);
	const std::vector<OpenGL3Mesh*>& getSubMeshesArray() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	bool hasPreparedVertexData() const;

	void addVertex(const Vertex&);
	void addIndex(uint32);
	void updateBuffers();

	GLuint getVertexArrayObjectPointer() const;
	size_t getVerticesCount() const;
	size_t getIndicesCount() const;
protected:
	std::vector<OpenGL3Mesh*> m_subMeshes;

	std::vector<Vertex> m_vertices;
	std::vector<uint32> m_indices;

	std::string m_name;
private:
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};