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

glm::vec2 OpenGL3Sprite::getPosition() const {
	return m_position;
}

void OpenGL3Sprite::setPosition(const glm::vec2& position) {
	m_position = position;
}

void OpenGL3Sprite::move(const glm::vec2& movement) {
	m_position += movement;
}

glm::vec2 OpenGL3Sprite::getSize() const {
	return m_size;
}

void OpenGL3Sprite::setSize(const glm::vec2& size) {
	m_size = size;
}

glm::mat4 OpenGL3Sprite::getTransformationMatrix() const {
	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(m_position, 0.0f));

	//matrix = glm::translate(matrix, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
	//matrix = glm::rotate(matrix, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//matrix = glm::translate(matrix, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));

	matrix = glm::scale(matrix, glm::vec3(m_size, 1.0f));

	return matrix;
}