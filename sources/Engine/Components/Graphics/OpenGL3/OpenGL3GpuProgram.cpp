#include "OpenGL3GpuProgram.h"

#include <unordered_map>
#include <Engine\Components\Debugging\Log.h>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Texture.h"

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
}

void OpenGL3GpuProgram::destroy() {
	glDeleteProgram(0);
	m_program = 0;
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
		GLchar message[512];
		glGetShaderInfoLog(shader, sizeof(message), NULL, message);

		throw std::exception(message);
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
		GLchar message[512];
		glGetProgramInfoLog(m_program, sizeof(message), NULL, message);
		
		throw std::exception(message);
	}

	if (m_vertexShader != 0) {
		glDeleteShader(m_vertexShader);
		m_vertexShader = 0;
	}

	if (m_fragmentShader != 0) {
		glDeleteShader(m_fragmentShader);
		m_fragmentShader = 0;
	}
}

GLuint OpenGL3GpuProgram::getGpuProgramPointer() const {
	return m_program;
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, bool parameterValue) {
	glUniform1i(glGetUniformLocation(m_program, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, int parameterValue) {
	glUniform1i(glGetUniformLocation(m_program, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, float parameterValue) {
	glUniform1f(glGetUniformLocation(m_program, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const vector3& parameterValue) {
	glUniform3fv(glGetUniformLocation(m_program, parameterName.c_str()), 1, &parameterValue[0]);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const matrix4& parameterValue) {
	glUniformMatrix4fv(glGetUniformLocation(m_program, parameterName.c_str()), 1, GL_FALSE, &parameterValue[0][0]);
}

void OpenGL3GpuProgram::setParameter(const std::string& name, Texture* value) {
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), static_cast<OpenGL3Texture*>(value)->getTexturePointer());
}