#pragma once

#include <string>
#include "OpenGL3.h"
#include "../Math.h"

typedef GLuint shaderId;

class OpenGL3Shader {
public:
	OpenGL3Shader();
	~OpenGL3Shader();

	void loadFromFile(const std::string&);
	void unload();

	shaderId getShaderPointer() const;

	void setInteger(const std::string&, int) const;
	void setMatrix4(const std::string&, const glm::mat4&) const;
	void setVector3(const std::string&, const glm::vec3&) const;
private:
	shaderId m_programId;
};

typedef OpenGL3Shader Shader;