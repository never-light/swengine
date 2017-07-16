#include "OpenGL3Shader.h"

#include <fstream>
#include <iostream>
#include <Engine\Components\Debugging\Log.h>

enum ReadShaderCodeState {
	READ_UNDEFINED_SHADER_CODE, READ_VERTEX_SHADER_CODE, READ_FRAGMENT_SHADER_CODE
};

OpenGL3Shader::OpenGL3Shader() : m_programId(NULL) {

}

OpenGL3Shader::~OpenGL3Shader() {
	unload();
}

void OpenGL3Shader::loadFromFile(const std::string& filename) {
	std::ifstream inputFile(filename);
	std::string vertexShaderCodeString, fragmentShaderCodeString;
	std::string line;

	ReadShaderCodeState readShaderCodeState = READ_UNDEFINED_SHADER_CODE;

	while (std::getline(inputFile, line)) {
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

	// Çàãðóçêà âåðøèííîãî øåéäåðà
	const char* vertexShaderCode = vertexShaderCodeString.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	this->checkLoadingShaderError(vertexShader);

	// Çàãðóçêà ôðàãìåíòíîãî øåéäåðà
	const char* fragmentShaderCode = fragmentShaderCodeString.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	//Chek Errors
	this->checkLoadingShaderError(fragmentShader);

	this->m_programId = glCreateProgram();

	glAttachShader(this->m_programId, vertexShader);
	glAttachShader(this->m_programId, fragmentShader);
	glLinkProgram(this->m_programId);

	this->checkCreatingShaderProgramError(this->m_programId);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void OpenGL3Shader::checkLoadingShaderError(GLuint shader) {
	int compileStatus;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		errlog() << infoLog;
	}

}

void OpenGL3Shader::checkCreatingShaderProgramError(shaderId program) {
	int linkStatus;

	glGetShaderiv(program, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		errlog() << infoLog;
	}
}

void OpenGL3Shader::unload() {
	if (m_programId) {
		glDeleteProgram(this->m_programId);
	}
}

shaderId OpenGL3Shader::getShaderPointer() const {
	return m_programId;
}

void OpenGL3Shader::setInteger(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void OpenGL3Shader::setMatrix4(const std::string& name, const glm::mat4& matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void OpenGL3Shader::setVector3(const std::string& name, const glm::vec3& vector) const {
	glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &vector[0]);
}
