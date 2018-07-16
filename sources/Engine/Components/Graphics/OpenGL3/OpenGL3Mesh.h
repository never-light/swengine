#pragma once

#include <Engine\Components\Graphics\RenderSystem\Mesh.h>
#include "OpenGL3Buffer.h"

class OpenGL3Mesh : public Mesh {
public:
	OpenGL3Mesh();
	virtual ~OpenGL3Mesh();
	virtual void render() override;

	virtual void create() override;
	virtual void destroy() override;

protected:
	OpenGL3Buffer* m_vertexBuffer;
	OpenGL3Buffer* m_indexBuffer;

	GLuint m_VAO;
};