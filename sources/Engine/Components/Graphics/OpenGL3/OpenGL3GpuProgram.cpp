#include "OpenGL3GpuProgram.h"

#include <unordered_map>
#include <Engine\Components\Debugging\Log.h>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Texture.h"

const std::unordered_map<std::string, GLenum> OpenGL3GpuProgram::m_availableShaders {
	{ "vertexShader", GL_VERTEX_SHADER },
	{ "fragmentShader", GL_FRAGMENT_SHADER },
};

OpenGL3GpuProgram::OpenGL3GpuProgram(const std::unordered_map<std::string, std::string>& sources) 
	: GpuProgram(sources), 
	m_programId(NULL) 
{
	m_programId = glCreateProgram();

	std::unordered_map<std::string, GLenum> shaders;

	for (auto it : m_availableShaders) {
		auto sourcesIt = sources.find(it.first);
		if (sourcesIt != sources.end()) {
			GLuint shader = compileShader(sourcesIt->second, it.second);
			glAttachShader(this->m_programId, shader);

			shaders[sourcesIt->first] = shader;
		}
	}

	glLinkProgram(this->m_programId);

	checkProgramLinkingState();

	for (auto it : shaders) {
		glDeleteShader(it.second);
	}
}

OpenGL3GpuProgram::~OpenGL3GpuProgram() {
	if (m_programId) {
		glDeleteProgram(this->m_programId);
	}
}

GLuint OpenGL3GpuProgram::compileShader(const std::string& source, GLenum type) {
	const char* sourceText = source.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &sourceText, NULL);
	glCompileShader(shader);

	checkShaderState(shader);

	return shader;
}

void OpenGL3GpuProgram::checkShaderState(GLuint shader) {
	int compileStatus;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		errlog() << infoLog;
		throw std::exception();
	}
}

void OpenGL3GpuProgram::checkProgramLinkingState() {
	int linkStatus;

	glGetShaderiv(m_programId, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus) {
		char infoLog[512];
		glGetProgramInfoLog(m_programId, 512, NULL, infoLog);

		errlog() << infoLog;
		throw std::exception();
	}
}

GLuint OpenGL3GpuProgram::getShaderPointer() const {
	return m_programId;
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, bool parameterValue) {
	glUniform1i(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, int parameterValue) {
	glUniform1i(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, float32 parameterValue) {
	glUniform1f(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const vector3& parameterValue) {
	glUniform3fv(glGetUniformLocation(m_programId, parameterName.c_str()), 1, &parameterValue[0]);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const matrix4& parameterValue) {
	glUniformMatrix4fv(glGetUniformLocation(m_programId, parameterName.c_str()), 1, GL_FALSE, &parameterValue[0][0]);
}

void OpenGL3GpuProgram::setParameter(const std::string& name, Texture* value) {
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), static_cast<OpenGL3Texture*>(value)->getTexturePointer());
}

void OpenGL3GpuProgram::attachUniformBlock(const std::string& name, GLuint bindingPoint) {
	glUniformBlockBinding(m_programId, glGetUniformBlockIndex(m_programId, name.c_str()), bindingPoint);
}

const std::vector<GpuProgramRequiredParameter>& OpenGL3GpuProgram::getRequiredParameters() const {
	return m_requiredParameters;
}

void OpenGL3GpuProgram::setRequiredParameters(const std::vector<GpuProgramRequiredParameter>& parameters) {
	m_requiredParameters = parameters;
}