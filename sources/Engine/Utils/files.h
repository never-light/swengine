#pragma once

#include <string>

class FilesUtils {
public:
	static bool isExists(const std::string& filename);
	
private:
	FilesUtils() = delete;
	~FilesUtils() = delete;

	FilesUtils(const FilesUtils& other) = delete;
};