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

	void setShader(Shader*);
	Shader* getShader() const;

	GLuint getVertexArrayPointer() const;
private:
	const Texture* m_texture;
	Shader* m_shaderProgram;

	GLuint m_VertexArrayObject;
	GLuint m_VertexBufferObject;
};

typedef OpenGL3Sprite Sprite;