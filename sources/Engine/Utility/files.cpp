#include "precompiled.h"

#pragma hdrstop

#include "files.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Exceptions/EngineRuntimeException.h"

bool FileUtils::isFileExists(const std::string& path)
{
  std::ifstream file;
  file.open(path);
  file.close();

  return static_cast<bool>(file);
}

std::string FileUtils::readFile(const std::string& path)
{
  std::ifstream file(path);
  return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
