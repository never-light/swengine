#pragma once

#include <string>

class IOUtils {
public:
	static void copyToClipboard(const std::string& string);

private:
	IOUtils() = delete;
	~IOUtils() = delete;

	IOUtils(const IOUtils& other) = delete;
};