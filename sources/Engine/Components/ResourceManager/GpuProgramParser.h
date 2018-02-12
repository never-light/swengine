#pragma once

#include <string>
#include <unordered_map>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>

class GpuProgramParser {
public:
	GpuProgramParser(const std::string& source);
	~GpuProgramParser();

	const std::unordered_map<std::string, std::string>& getShadersSources() const;
	const std::vector<GpuProgramRequiredParameter>& getRequiredParameters() const;
private:
	std::unordered_map<std::string, std::string> m_shadersSources;
	std::vector<GpuProgramRequiredParameter> m_requiredParameters;
};