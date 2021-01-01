#pragma once

#include <string>
#include <string_view>
#include <vector>

class FileUtils {
 public:
  [[nodiscard]] static bool isDirExists(const std::string& path);
  [[nodiscard]] static bool isFileExists(const std::string& path);
  [[nodiscard]] static std::string readFile(const std::string& path);

  [[nodiscard]] static std::string getAbsolutePath(const std::string& relativePath);
  [[nodiscard]] static std::string getResourcePath(const std::string& resourceLocalPath);
  [[nodiscard]] static std::string getGameResourcePath(const std::string& resourceLocalPath);

  [[nodiscard]] static std::string getLevelPath(const std::string& levelName);
  [[nodiscard]] static std::string getSpawnListPath(const std::string& spawnListName);
  [[nodiscard]] static std::string getGUISchemePath(const std::string& schemeName);
  [[nodiscard]] static std::string getScriptPath(const std::string& scriptName);

  [[nodiscard]] static std::string getSavePath(const std::string& saveName);


  [[nodiscard]] static std::vector<std::string> getScriptsList();

 public:
  static constexpr std::string_view WORKING_DIR = ".";
  static constexpr std::string_view BIN_PATH = FileUtils::WORKING_DIR;
  static constexpr std::string_view RESOURCES_PATH = "./../resources";
  static constexpr std::string_view RESOURCES_GAME_PATH = "./../resources/game";
  static constexpr std::string_view LEVELS_PATH = "./../resources/game/levels";
  static constexpr std::string_view GUI_SCHEMES_PATH = "./../resources/game/gui_schemes";
  static constexpr std::string_view SPAWN_LISTS_PATH = "./../resources/game/spawn";
  static constexpr std::string_view SCRIPTS_PATH = "./../resources/game/scripts";
  static constexpr std::string_view SAVES_PATH = "./saves";

  static constexpr std::string_view STARTUP_SETTINGS_PATH = "settings.xml";
};
