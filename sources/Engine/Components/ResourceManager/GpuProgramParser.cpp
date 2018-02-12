#include "GpuProgramParser.h"
#include "ParseUtils.h"

#include <sstream>
#include <regex>

GpuProgramParser::GpuProgramParser(const std::string& source) {
	auto sections = ParseSplitStrBySections(source);

	if (sections.find("parameters") != sections.end()) {
		std::stringstream parametersStream(sections["parameters"]);
		std::string part;

		std::regex parameterRegex("^([a-zA-Z0-9_]+)\\s+([a-zA-Z0-9_\\.]+)\\s+as\\s+([a-zA-Z0-9_\\.]+)$");

		std::smatch match;

		while (std::getline(parametersStream, part, '\n')) {
			if (part.empty())
				continue;

			if (!std::regex_search(part, match, parameterRegex))
				throw std::exception();

			m_requiredParameters.push_back(GpuProgramRequiredParameter(match[1], match[2], match[3]));
		}
	}

	std::vector<std::string> m_availableShaderSections{ "vertexShader", "fragmentShader" };

	for (auto& sectionName : m_availableShaderSections) {
		auto sectionIt = sections.find(sectionName);

		if (sectionIt != sections.end())
			m_shadersSources[sectionName] = sectionIt->second;
	}
}

GpuProgramParser::~GpuProgramParser() {

}

const std::unordered_map<std::string, std::string>& GpuProgramParser::getShadersSources() const {
	return m_shadersSources;
}

const std::vector<GpuProgramRequiredParameter>& GpuProgramParser::getRequiredParameters() const {
	return m_requiredParameters;
}