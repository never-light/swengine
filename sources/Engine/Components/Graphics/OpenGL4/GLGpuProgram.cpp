#include "GLGpuProgram.h"

#include <unordered_map>
#include <Engine/types.h>
#include <Engine/Components/Math/Math.h>
#include "GLTexture.h"

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>
#include "GLDebug.h"

using namespace std::string_literals;

GLGpuProgram::GLGpuProgram() 
	: GpuProgram(), m_program(0), m_vertexShader(0), m_fragmentShader(0)
{

}

GLGpuProgram::~GLGpuProgram() {
	if (m_program != 0)
		destroy();
}

void GLGpuProgram::bind() {
	glUseProgram(m_program);
}

void GLGpuProgram::unbind() {
	glUseProgram(0);
}

void GLGpuProgram::create() {
	m_program = glCreateProgram();

	if (m_program == 0)
		throw RenderSystemException("Failed to create OpenGL GPU program", __FILE__, __LINE__, __FUNCTION__);
}

void GLGpuProgram::destroy() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

void GLGpuProgram::addShader(ShaderType type, const std::string &source) {
	const char* rawSource = source.c_str();
	GLenum shaderType = (type == ShaderType::Vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &rawSource, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string message;
		message.resize(infoLogLength);

		glGetShaderInfoLog(shader, infoLogLength, NULL, &message[0]);

		throw RenderSystemException(("Failed to compile shader: " + message).c_str(), __FILE__, __LINE__, __FUNCTION__);
	}

	glAttachShader(m_program, shader);

	if (type == ShaderType::Vertex)
		m_vertexShader = shader;
	else
		m_fragmentShader = shader;
}

void GLGpuProgram::link() {
	glLinkProgram(m_program);

	GLint status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);

	if (!status) {
		GLint infoLogLength;
		glGetShaderiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string message;
		message.resize(infoLogLength);

		glGetProgramInfoLog(m_program, infoLogLength, NULL, &message[0]);
		
		throw RenderSystemException(("Failed to link GPU program: " + message).c_str(), __FILE__, __LINE__, __FUNCTION__);
	}

	glDeleteShader(m_vertexShader);
	m_vertexShader = 0;

	glDeleteShader(m_fragmentShader);
	m_fragmentShader = 0;
}

GLuint GLGpuProgram::getGpuProgramPointer() const {
	return m_program;
}

void GLGpuProgram::setParameter(const std::string& parameterName, bool parameterValue) {
	GL_CALL(glUniform1i(getUniformLocation(parameterName), parameterValue));
}

void GLGpuProgram::setParameter(const std::string& parameterName, int parameterValue) {
	GL_CALL(glUniform1i(getUniformLocation(parameterName), parameterValue));
}

void GLGpuProgram::setParameter(const std::string& parameterName, float parameterValue) {
	GL_CALL(glUniform1f(getUniformLocation(parameterName), parameterValue));
}

void GLGpuProgram::setParameter(const std::string & parameterName, const vector2 & parameterValue)
{
	GL_CALL(glUniform2fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void GLGpuProgram::setParameter(const std::string& parameterName, const vector3& parameterValue) {
	GL_CALL(glUniform3fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void GLGpuProgram::setParameter(const std::string& parameterName, const vector4& parameterValue) {
	GL_CALL(glUniform4fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void GLGpuProgram::setParameter(const std::string& parameterName, const matrix4& parameterValue) {
	GL_CALL(glUniformMatrix4fv(getUniformLocation(parameterName), 1, GL_FALSE, &parameterValue[0][0]));
}

GLint GLGpuProgram::getUniformLocation(const std::string & name) const
{
	GLint location = glGetUniformLocation(m_program, name.c_str());

	//if (location == -1)
		//throw RenderSystemException(("Failed to set uniform value: invalid uniform name [" + name + "]").c_str(), __FILE__, __LINE__, __FUNCTION__);

	return location;
}