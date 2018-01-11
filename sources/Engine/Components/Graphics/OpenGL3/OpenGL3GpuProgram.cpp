#include "OpenGL3GpuProgram.h"

#include <sstream>
#include <iostream>
#include <Engine\Components\Debugging\Log.h>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

enum ReadShaderCodeState {
	READ_UNDEFINED_SHADER_CODE, READ_VERTEX_SHADER_CODE, READ_FRAGMENT_SHADER_CODE
};

OpenGL3GpuProgram::OpenGL3GpuProgram(const std::string& source) : m_programId(NULL) {
	std::stringstream sourceStream(source);
	std::string vertexShaderCodeString, fragmentShaderCodeString;
	std::string line;

	ReadShaderCodeState readShaderCodeState = READ_UNDEFINED_SHADER_CODE;

	while (std::getline(sourceStream, line)) {
		if (line == "@vertexShader") {
			readShaderCodeState = READ_VERTEX_SHADER_CODE;
		}
		else if (line == "@fragmentShader") {
			readShaderCodeState = READ_FRAGMENT_SHADER_CODE;
		}
		else {
			if (readShaderCodeState == READ_VERTEX_SHADER_CODE) {
				vertexShaderCodeString += (line + "\n");
			}
			else if (readShaderCodeState == READ_FRAGMENT_SHADER_CODE) {
				fragmentShaderCodeString += (line + "\n");
			}
		}
	}

	GLuint vertexShader, fragmentShader;
	int success;

	const char* vertexShaderCode = vertexShaderCodeString.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	this->checkLoadingShaderError(vertexShader);

	const char* fragmentShaderCode = fragmentShaderCodeString.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	this->checkLoadingShaderError(fragmentShader);

	this->m_programId = glCreateProgram();

	glAttachShader(this->m_programId, vertexShader);
	glAttachShader(this->m_programId, fragmentShader);
	glLinkProgram(this->m_programId);

	this->checkCreatingShaderProgramError(this->m_programId);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

OpenGL3GpuProgram::~OpenGL3GpuProgram() {
	if (m_programId) {
		glDeleteProgram(this->m_programId);
	}
}

void OpenGL3GpuProgram::checkLoadingShaderError(GLuint shader) {
	int compileStatus;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		errlog() << infoLog;
	}

}

void OpenGL3GpuProgram::checkCreatingShaderProgramError(shaderId program) {
	int linkStatus;

	glGetShaderiv(program, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		errlog() << infoLog;
	}
}

shaderId OpenGL3GpuProgram::getShaderPointer() const {
	return m_programId;
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, bool parameterValue) const {
	glUniform1i(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, int parameterValue) const {
	glUniform1i(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, float32 parameterValue) const {
	glUniform1f(glGetUniformLocation(m_programId, parameterName.c_str()), parameterValue);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const vector3& parameterValue) const {
	glUniform3fv(glGetUniformLocation(m_programId, parameterName.c_str()), 1, &parameterValue[0]);
}

void OpenGL3GpuProgram::setParameter(const std::string& parameterName, const matrix4& parameterValue) const {
	glUniformMatrix4fv(glGetUniformLocation(m_programId, parameterName.c_str()), 1, GL_FALSE, &parameterValue[0][0]);
}