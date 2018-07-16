#include "OpenGL3Mesh.h"

#include <cstddef>
#include <Engine\Components\Debugging\Log.h>

OpenGL3Mesh::OpenGL3Mesh() 
	: Mesh(), m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_VAO(0)
{

}

OpenGL3Mesh::~OpenGL3Mesh()
{
	if (m_VAO != 0)
		destroy();
}

void OpenGL3Mesh::render()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		errlog() << error;
	}
}

void OpenGL3Mesh::create()
{
	m_vertexBuffer = new OpenGL3Buffer(Buffer::Type::Vertex);
	m_vertexBuffer->create();
	m_vertexBuffer->bind();

	size_t vertexBufferSize = sizeof(m_positions[0]) * m_positions.size() +
		sizeof(m_normals[0]) * m_normals.size() +
		sizeof(m_tangents[0]) * m_tangents.size() +
		sizeof(m_uv[0]) * m_uv.size();

	m_vertexBuffer->setData(vertexBufferSize, nullptr);

	m_vertexBuffer->setData(0, sizeof(m_positions[0]) * m_positions.size(), (const std::byte*)m_positions.data());
	m_vertexBuffer->setData(sizeof(m_positions[0]) * m_positions.size(), sizeof(m_normals[0]) * m_normals.size(), (const std::byte*)m_normals.data());
	
	m_vertexBuffer->setData(
		sizeof(m_positions[0]) * m_positions.size() + sizeof(m_normals[0]) * m_normals.size(), 
		sizeof(m_tangents[0]) * m_tangents.size(), (const std::byte*)m_tangents.data());

	m_vertexBuffer->setData(
		sizeof(m_tangents[0]) * m_tangents.size() + 
			sizeof(m_positions[0]) * m_positions.size() + sizeof(m_normals[0]) * m_normals.size(),
		sizeof(m_uv[0]) * m_uv.size(), (const std::byte*)m_uv.data());

	m_vertexBuffer->unbind();

	m_indexBuffer = new OpenGL3Buffer(Buffer::Type::Index);
	m_indexBuffer->create();
	m_indexBuffer->bind();

	m_indexBuffer->setData(sizeof(m_indices[0]) * m_indices.size(), (const std::byte*)m_indices.data());

	m_indexBuffer->unbind();

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	m_vertexBuffer->bind();

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(m_positions[0]) * m_positions.size()));

	// tangent attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(m_positions[0]) * m_positions.size() + sizeof(m_normals[0]) * m_normals.size()));

	// texture coordinates
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(
		sizeof(m_positions[0]) * m_positions.size() + sizeof(m_normals[0]) * m_normals.size() + sizeof(m_tangents[0]) * m_tangents.size()));


	m_indexBuffer->bind();

	glBindVertexArray(0);
	m_vertexBuffer->unbind();
	m_indexBuffer->unbind();
}

void OpenGL3Mesh::destroy()
{
	if (m_VAO != 0) {
		delete m_vertexBuffer;
		delete m_indexBuffer;

		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}
