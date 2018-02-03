#pragma once

#include <Engine\types.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

enum class MaterialDataTokenType {

};

struct MaterialDataToken {
	
};

class MaterialsDataParser {
public:
	using ConstructorArgValue = std::variant<int, real, bool, std::string>;
	using ConstructorArgsList = std::vector<ConstructorArgValue>;

	using ParameterValueConstructor = struct {
		std::string name;
		ConstructorArgsList args;
	};
	using ParameterValue = std::variant<int, real, bool, std::string, ParameterValueConstructor>;

	using ParametersList = std::unordered_map<std::string, ParameterValue>;
	using MaterialsDataList = std::unordered_map<std::string, ParametersList>;
public:
	MaterialsDataParser(const std::string& source);
	~MaterialsDataParser();

	const MaterialsDataList& getMaterialsData() const;
private:
	void parseSection(const std::string& name, const std::string& text);

private:
	MaterialsDataList m_materialsData;
};