#include "ParseUtils.h"

#include <sstream>

std::unordered_map<std::string, std::string> ParseSplitStrBySections(const std::string& str) {
	std::stringstream in(str);

	std::string currentSectionName;
	std::string currentLine;
	std::unordered_map<std::string, std::string> sections;

	while (std::getline(in, currentLine)) {
		if (currentLine.empty()) {
			continue;
		}

		if (currentLine[0] == '[') {
			size_t end = currentLine.find_first_of(']');
			if (end != std::string::npos) {
				currentSectionName = currentLine.substr(1, end - 1);

				sections[currentSectionName] = "";
				continue;
			}
			else {
				throw std::exception();
			}
		}
		else {
			if (currentSectionName.empty())
				throw std::exception();
		}

		sections[currentSectionName] += (currentLine + "\n");
	}

	return sections;
}