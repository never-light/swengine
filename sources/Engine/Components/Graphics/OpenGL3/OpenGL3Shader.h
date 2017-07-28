#pragma once

#include <string>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3.h"
#include <Engine\Components\Graphics\Color.h>
#include "OpenGL3Texture.h"
#include "OpenGL3Lightmap.h"

typedef GLuint shaderId;

class OpenGL3Shader {
public:
	OpenGL3Shader();
	~OpenGL3Shader();

	void loadFromFile(const std::string&);
	void unload();

	shaderId getShaderPointer() const;

	template<typename ParameterType>
	void setParameter(const std::string&, const ParameterType&);

	void setInteger(const std::string&, int) const;
	void setMatrix4(const std::string&, const glm::mat4&) const;
	void setVector3(const std::string&, const glm::vec3&) const;
private:
	void checkLoadingShaderError(GLuint);
	void checkCreatingShaderProgramError(shaderId);

	shaderId m_programId;
};

typedef OpenGL3Shader Shader;
