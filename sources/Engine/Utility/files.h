#pragma once

#include <string>
#include <string_view>

class FileUtils {
 public:
  [[nodiscard]] static bool isDirExists(const std::string& path);
  [[nodiscard]] static bool isFileExists(const std::string& path);
  [[nodiscard]] static std::string readFile(const std::string& path);

  [[nodiscard]] static std::string getAbsolutePath(const std::string& relativePath);
  [[nodiscard]] static std::string getResourcePath(const std::string& resourceLocalPath);

  [[nodiscard]] static std::string getLevelPath(const std::string& levelName);
  [[nodiscard]] static std::string getGUISchemePath(const std::string& schemeName);

 public:
  static constexpr std::string_view WORKING_DIR = ".";
  static constexpr std::string_view BIN_PATH = FileUtils::WORKING_DIR;
  static constexpr std::string_view RESOURCES_PATH = "./../resources";
  static constexpr std::string_view LEVELS_PATH = "./../resources/game/levels";
  static constexpr std::string_view GUI_SCHEMES_PATH = "./../resources/game/gui_schemes";
};
