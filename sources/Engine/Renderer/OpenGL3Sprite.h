#pragma once

#include "OpenGL3Texture.h"
#include "OpenGL3Shader.h"
#include "../Math.h"

class OpenGL3Sprite {
public:
	OpenGL3Sprite();
	~OpenGL3Sprite();

	void setTexture(const Texture*);
	const Texture* getTexture() const;

	glm::vec2 getPosition() const;
	void setPosition(const glm::vec2&);
	void move(const glm::vec2&);
	
	glm::vec2 getSize() const;
	void setSize(const glm::vec2&);

	glm::mat4 getTransformationMatrix() const;

	void setShader(Shader*);
	Shader* getShader() const;

	GLuint getVertexArrayPointer() const;
private:
	const Texture* m_texture;
	Shader* m_shaderProgram;

	GLuint m_VertexArrayObject;
	GLuint m_VertexBufferObject;

	glm::vec2 m_position;
	glm::vec2 m_size;
};

typedef OpenGL3Sprite Sprite;