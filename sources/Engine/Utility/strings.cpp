#include "precompiled.h"

#pragma hdrstop

#include "strings.h"

#include <sstream>
#include <algorithm>

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter)
{
  std::vector<std::string> entries;
  std::string entry;

  std::istringstream stringStream(str);

  while (std::getline(stringStream, entry, delimiter)) {
    entries.push_back(entry);
  }

  return entries;
}

std::string StringUtils::toLowerCase(const std::string& str)
{
  std::string result;

  for (auto ch : str) {
    result.push_back(static_cast<char>(std::tolower(ch)));
  }

  return result;
}

std::string StringUtils::regexReplace(const std::string& expression, std::string str,
  const std::function<std::string(const std::smatch&)>& callback)
{
  std::regex regexp(expression);
  std::smatch match;

  std::string result;

  while (regex_search(str, match, regexp)) {
    result += std::string(match.prefix()) + callback(match);
    str = match.suffix();
  }

  result += str;

  return result;
}

std::string StringUtils::replace(std::string source,
  const std::string& pattern,
  const std::string& replacement)
{
  size_t position = source.find(pattern);

  while (position != std::string::npos) {
    source.replace(position, pattern.length(), replacement);
    position = source.find(pattern, position + replacement.length());
  }

  return source;
}

glm::vec2 StringUtils::stringToVec2(const std::string& string)
{
  auto parts = split(string, ' ');

  return glm::vec2(std::stof(parts[0]), std::stof(parts[1]));
}

glm::vec3 StringUtils::stringToVec3(const std::string& string)
{
  auto parts = split(string, ' ');

  return glm::vec3(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
}

glm::vec4 StringUtils::stringToVec4(const std::string& string)
{
  auto parts = split(string, ' ');

  return glm::vec4(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]));
}

glm::ivec2 StringUtils::stringToIVec2(const std::string& string)
{
  auto parts = split(string, ' ');

  return glm::ivec2(std::stoi(parts[0]), std::stoi(parts[1]));
}
