#pragma once

#include <Engine\Components\Math\Math.h>
#include "OpenGL3Texture.h"
#include "OpenGL3Shader.h"

class OpenGL3Sprite {
public:
	OpenGL3Sprite();
	~OpenGL3Sprite();

	void setTexture(const OpenGL3Texture*);
	const OpenGL3Texture* getTexture() const;

	void setShader(OpenGL3Shader*);
	OpenGL3Shader* getShader() const;

	GLuint getVertexArrayPointer() const;
private:
	const OpenGL3Texture* m_texture;
	OpenGL3Shader* m_shaderProgram;

	GLuint m_VertexArrayObject;
	GLuint m_VertexBufferObject;
};