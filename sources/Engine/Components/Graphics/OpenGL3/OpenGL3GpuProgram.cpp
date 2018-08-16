#include "OpenGL3GpuProgram.h"

#include <unordered_map>
#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Texture.h"

#include <Engine\Components\Graphics\RenderSystem\RenderSystemException.h>
#include "OpenGL3Errors.h"

using namespace std::string_literals;

OpenGL3GpuProgram::OpenGL3GpuProgram() 
	: GpuProgram(), m_program(0), m_vertexShader(0), m_fragmentShader(0)
{

}

OpenGL3GpuProgram::~OpenGL3GpuProgram() {
	if (m_program != 0)
		destroy();
}

void OpenGL3GpuProgram::bind() {
	glUseProgram(m_program);
}

void OpenGL3GpuProgram::unbind() {
	glUseProgram(0);
}

void OpenGL3GpuProgram::create() {
	m_program = glCreateProgram();

	if (m_program == 0)
		throw RenderSystemException("Failed to create OpenGL GPU program", __FILE__, __LINE__, __FUNCTION__);
}

void OpenGL3GpuProgram::destroy() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

void OpenGL3GpuProgram::addShader(ShaderType type, const std::string &source) {
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

void OpenGL3GpuProgram::link() {
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

GLuint OpenGL3GpuProgram::getGpuProgramPointer() const {
	return m_program;
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, bool parameterValue) {
	OPENGL3_CALL(glUniform1i(getUniformLocation(parameterName), parameterValue));
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, int parameterValue) {
	OPENGL3_CALL(glUniform1i(getUniformLocation(parameterName), parameterValue));
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, float parameterValue) {
	OPENGL3_CALL(glUniform1f(getUniformLocation(parameterName), parameterValue));
}

void OpenGL3GpuProgram::setParameter(const std::string & parameterName, const vector2 & parameterValue)
{
	OPENGL3_CALL(glUniform2fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const vector3& parameterValue) {
	OPENGL3_CALL(glUniform3fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const vector4& parameterValue) {
	OPENGL3_CALL(glUniform4fv(getUniformLocation(parameterName), 1, &parameterValue[0]));
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const matrix4& parameterValue) {
	OPENGL3_CALL(glUniformMatrix4fv(getUniformLocation(parameterName), 1, GL_FALSE, &parameterValue[0][0]));
}

GLint OpenGL3GpuProgram::getUniformLocation(const std::string & name) const
{
	GLint location = glGetUniformLocation(m_program, name.c_str());

	if (location == -1)
		throw RenderSystemException(("Failed to set uniform value: invalid uniform name [" + name + "]").c_str(), __FILE__, __LINE__, __FUNCTION__);

	return location;
}