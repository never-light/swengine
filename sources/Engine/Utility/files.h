#pragma once

#include <string>

class FileUtils {
public:
    static bool isFileExists(const std::string& path);
    static std::string readFile(const std::string& path);
};
