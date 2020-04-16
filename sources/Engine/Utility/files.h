#pragma once

#include <string>

class FileUtils {
 public:
  [[nodiscard]] static bool isFileExists(const std::string& path);
  [[nodiscard]] static std::string readFile(const std::string& path);
};
