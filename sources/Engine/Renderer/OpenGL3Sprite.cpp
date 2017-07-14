#include "OpenGL3Sprite.h"
#include "../utils/utils.h"

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
	infolog() << "Спрайт выгружен";
	glDeleteVertexArrays(1, &m_VertexArrayObject);
	glDeleteBuffers(1, &m_VertexBufferObject);
}

void OpenGL3Sprite::setTexture(const Texture* texture) {
	m_texture = texture;
}

void OpenGL3Sprite::setShader(Shader* shader) {
	m_shaderProgram = shader;
}

Shader* OpenGL3Sprite::getShader() const {
	return m_shaderProgram;
}

const Texture* OpenGL3Sprite::getTexture() const {
	return m_texture;
}

GLuint OpenGL3Sprite::getVertexArrayPointer() const {
	return m_VertexArrayObject;
}