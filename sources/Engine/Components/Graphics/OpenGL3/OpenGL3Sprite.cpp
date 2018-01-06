#include "OpenGL3Sprite.h"
#include <Engine\Components\Debugging\Log.h>

OpenGL3Sprite::OpenGL3Sprite() 
	: m_texture(nullptr), m_VertexArrayObject(NULL), m_VertexBufferObject(NULL) {
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_VertexArrayObject);
	glGenBuffers(1, &m_VertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VertexArrayObject);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

OpenGL3Sprite::~OpenGL3Sprite() {
	glDeleteVertexArrays(1, &m_VertexArrayObject);
	glDeleteBuffers(1, &m_VertexBufferObject);
}

void OpenGL3Sprite::setTexture(const Texture* texture) {
	m_texture = static_cast<const OpenGL3Texture*>(texture);
}

const Texture* OpenGL3Sprite::getTexture() const {
	return m_texture;
}

void OpenGL3Sprite::setShader(GpuProgram* shader) {
	m_shaderProgram = static_cast<OpenGL3GpuProgram*>(shader);;
}

GpuProgram* OpenGL3Sprite::getShader() const {
	return m_shaderProgram;
}

GLuint OpenGL3Sprite::getVertexArrayPointer() const {
	return m_VertexArrayObject;
}