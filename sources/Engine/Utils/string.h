#pragma once

#include <string>
#include <vector>
#include <sstream>

class StringUtils {
public:
	template<typename Out>
	static void split(const std::string& s, char delim, Out result, bool skipEmptyEntries);

	static std::vector<std::string> split(const std::string &s, char delim, bool skipEmptyEntries = false);

	static std::string format(const char *fmt, ...);

private:
	StringUtils() = delete;
	~StringUtils() = delete;

	StringUtils(const StringUtils& other) = delete;
};

template<typename Out>
inline void StringUtils::split(const std::string & s, char delim, Out result, bool skipEmptyEntries)
{
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		if (!item.empty() || !skipEmptyEntries)
			*(result++) = item;
	}
}
