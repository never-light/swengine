#include "OpenGL3GpuProgram.h"

#include <set>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iostream>
#include <Engine\Components\Debugging\Log.h>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

OpenGL3GpuProgram::OpenGL3GpuProgram(const std::string& source) : m_programId(NULL) {
	std::stringstream sourceStream(source);
	std::string line;

	std::unordered_map<std::string, std::string> sectionsText;

	std::string currentSection = "undefined";

	while (std::getline(sourceStream, line)) {
		if (line == "@vertexShader") {
			currentSection = "vertex";
		}
		else if (line == "@fragmentShader") {
			currentSection = "fragment";
		}
		else if (line == "@parameters") {
			currentSection = "parameters";
		}
		else {
			if (sectionsText.find(currentSection) == sectionsText.end())
				sectionsText.insert({ currentSection, "" });

			sectionsText[currentSection] += (line + "\n");
		}
	}

	if (sectionsText.find("parameters") != sectionsText.end()) {
		std::stringstream parametersStream(sectionsText["parameters"]);
		std::string part;

		std::regex parameterRegex("^([a-zA-Z0-9_]+)\\s+([a-zA-Z0-9_\\.]+)\\s+as\\s+([a-zA-Z0-9_\\.]+)$");

		std::smatch match;

		while (std::getline(parametersStream, part, '\n')) {
			if (part.empty())
				continue;

			if (!std::regex_search(part, match, parameterRegex))
				throw std::exception();

			std::string parameterType = match[1];
			std::string parameterLocation = match[2];
			std::string parameterAlias = match[3];

			size_t dotPosition = parameterLocation.find_first_of('.');

			if (dotPosition == std::string::npos)
				throw std::exception();

			std::string section = parameterLocation.substr(0, dotPosition);
			std::string name = parameterLocation.substr(dotPosition + 1);

			if (m_requiredParameters.find(section) == m_requiredParameters.end())
				m_requiredParameters.insert({ section, GpuProgramParametersSection() });

			if (m_requiredParameters[section].find(name) != m_requiredParameters[section].end())
				throw std::exception();

			m_requiredParameters[section][name] = GpuProgramParameter(
				std::string(match[1]), // Type
				std::string(match[2]), // Location
				std::string(match[3]) // Alias
			);
		}
	}

	GLuint vertexShader, fragmentShader;
	int success;

	const char* vertexShaderCode = sectionsText["vertex"].c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	this->checkLoadingShaderError(vertexShader);

	const char* fragmentShaderCode = sectionsText["fragment"].c_str();

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

const GpuProgramParametersList& OpenGL3GpuProgram::getRequiredParameters() const {
	return m_requiredParameters;
}

const GpuProgramParametersSection& OpenGL3GpuProgram::getRequiredParametersSection(const std::string& name) const {
	return m_requiredParameters.at(name);
}

bool OpenGL3GpuProgram::hasRequiredParametersSection(const std::string& name) const {
	return (m_requiredParameters.find(name) != m_requiredParameters.end());
}