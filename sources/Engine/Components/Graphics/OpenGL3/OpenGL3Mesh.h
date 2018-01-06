#pragma once

#include <Engine\Components\Graphics\RenderSystem\Mesh.h>
#include "OpenGL3.h"

class OpenGL3Mesh : public Mesh {
public:
	OpenGL3Mesh();
	OpenGL3Mesh(const std::vector<Vertex>&);
	OpenGL3Mesh(const std::vector<Vertex>&, const std::vector<uint32>&);
	virtual ~OpenGL3Mesh();

	void addSubMesh(Mesh* mesh) override;
	Mesh* getSubMesh(size_t index) override;
	const std::vector<Mesh*>& getSubMeshesArray() const override;

	void setName(const std::string& name) override;
	const std::string& getName() const override;

	void addVertex(const Vertex&) override;
	void addIndex(uint32) override;

	size_t getVerticesCount() const override;
	size_t getIndicesCount() const override;

	void updateBuffers();
	bool hasPreparedVertexData() const;
	GLuint getVertexArrayObjectPointer() const;

protected:
	std::vector<Mesh*> m_subMeshes;

	std::vector<Vertex> m_vertices;
	std::vector<uint32> m_indices;

	std::string m_name;
private:
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};