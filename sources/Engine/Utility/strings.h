#pragma once

#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <spdlog/spdlog.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Exceptions/exceptions.h"

class StringUtils {
 public:
  StringUtils() = delete;

  [[nodiscard]] static std::vector<std::string> split(const std::string& str, char delimiter);
  [[nodiscard]] static std::string toLowerCase(const std::string& str);

  template<class T>
  [[nodiscard]] static T filterValue(const std::string& rawValue,
    const std::unordered_map<std::string, T>& allowedValues, T defaultValue);

  [[nodiscard]] static std::string regexReplace(const std::string& expression, std::string str,
    const std::function<std::string(const std::smatch&)>& callback);

  [[nodiscard]] static std::string replace(std::string source,
    const std::string& pattern,
    const std::string& replacement);

  [[nodiscard]] static glm::vec2 stringToVec2(const std::string& string);
  [[nodiscard]] static glm::vec3 stringToVec3(const std::string& string);
  [[nodiscard]] static glm::vec4 stringToVec4(const std::string& string);

  [[nodiscard]] static glm::ivec2 stringToIVec2(const std::string& string);
};

template<class T>
T StringUtils::filterValue(const std::string& rawValue,
  const std::unordered_map<std::string, T>& allowedValues, T defaultValue)
{
  T value = defaultValue;

  if (!rawValue.empty()) {
    auto valueIt = allowedValues.find(rawValue);

    if (valueIt == allowedValues.end()) {
      THROW_EXCEPTION(EngineRuntimeException, "The value has invalid format");
    }

    value = valueIt->second;
  }

  return value;
}
