#include "MaterialsDataParser.h"
#include <sstream>
#include <regex>
#include <Engine\Components\Debugging\Log.h>

#include "ParseUtils.h"

MaterialsDataParser::MaterialsDataParser(const std::string& source) {
	auto sections = ParseSplitStrBySections(source);

	for (auto& it : sections) {
		parseSection(it.first, it.second);
	}
}

MaterialsDataParser::~MaterialsDataParser() {

}

const MaterialsDataParser::MaterialsDataList& MaterialsDataParser::getMaterialsData() const {
	return m_materialsData;
}

void MaterialsDataParser::parseSection(const std::string& name, const std::string& text) {
	m_materialsData[name] = ParametersList();

	std::regex parametersRegex("([a-zA-Z0-9_]+)\\s+=\\s+((true|false)|([-+]?[0-9]*\\.?[0-9]+)|\\\"([a-zA-Z0-9\\.\\/_\\\\]+)\\\"|(([a-zA-Z0-9_]+)\\([\\s]*((.|\\s)*?)[\\s]*\\)))");

	std::sregex_iterator next(text.begin(), text.end(), parametersRegex);
	std::sregex_iterator end;
	while (next != end) {
		std::smatch match = *next;

		std::string parameterName = match[1];
		ParameterValue parameterValue;

		// Bool
		if (!match[3].str().empty()) {
			parameterValue = (match[3] == "true");
		}
		// Number
		else if (!match[4].str().empty()) {
			// Real
			if (match[4].str().find_first_of('.') != std::string::npos)
				parameterValue = std::stof(match[4]);
			// Interger
			else
				parameterValue = std::stoi(match[4]);
		}
		// String
		else if (!match[5].str().empty()) {
			parameterValue = match[5];
		}
		// Constructor
		else if (!match[7].str().empty()) {
			ParameterValueConstructor constructor;
			constructor.name = match[7];

			if (!match[8].str().empty()) {
				std::string argsStr = match[8];

				std::regex constructorArgsRegEx("((true|false)|([-+]?[0-9]*\\.?[0-9]+)|\\\"([a-zA-Z0-9\\.\\/_\\\\]+)\\\")");

				std::sregex_iterator argsNext(argsStr.begin(), argsStr.end(), constructorArgsRegEx);
				std::sregex_iterator argsEnd;
				while (argsNext != argsEnd) {
					std::smatch match = *argsNext;

					ConstructorArgValue argValue;

					// Bool
					if (!match[2].str().empty()) {
						argValue = (match[2] == "true");
					}
					// Number
					else if (!match[3].str().empty()) {
						// Real
						if (match[3].str().find_first_of('.') != std::string::npos)
							argValue = std::stof(match[3]);
						// Interger
						else
							argValue = std::stoi(match[3]);
					}
					// String
					else if (!match[4].str().empty()) {
						argValue = match[4];
					}
					else {
						throw std::exception();
					}

					constructor.args.push_back(argValue);

					argsNext++;
				}
			}

			parameterValue = constructor;
		}
		else {
			throw std::exception();
		}

		m_materialsData[name][parameterName] = parameterValue;

		next++;
	}
}