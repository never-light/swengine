#include "precompiled.h"
#pragma hdrstop

#include "files.h"

#include <fstream>
#include <filesystem>

bool FileUtils::isFileExists(const std::string& path)
{
  return std::filesystem::exists(path);
}

bool FileUtils::isDirExists(const std::string& path)
{
  return std::filesystem::exists(path);
}

std::string FileUtils::readFile(const std::string& path)
{
  std::ifstream file(path);
  return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string FileUtils::getAbsolutePath(const std::string& relativePath)
{
  return std::filesystem::absolute(relativePath).string();
}

std::string FileUtils::getResourcePath(const std::string& resourceLocalPath)
{
  return std::string(RESOURCES_PATH) + "/" + resourceLocalPath;
}

std::string FileUtils::getLevelPath(const std::string& levelName)
{
  return std::string(LEVELS_PATH) + "/" + levelName;
}

std::string FileUtils::getGUISchemePath(const std::string& schemeName)
{
  return std::string(GUI_SCHEMES_PATH) + "/" + schemeName + ".xml";
}

std::string FileUtils::getGameResourcePath(const std::string& resourceLocalPath)
{
  return std::string(RESOURCES_GAME_PATH) + "/" + resourceLocalPath;
}

std::string FileUtils::getSpawnListPath(const std::string& spawnListName)
{
  return std::string(SPAWN_LISTS_PATH) + "/" + spawnListName + ".xml";
}
