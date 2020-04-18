#pragma once

#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <spdlog/spdlog.h>

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
                                  std::function<std::string(const std::smatch&)> callback);

  [[nodiscard]] static std::string replace(std::string source,
                             const std::string& pattern,
                             const std::string& replacement);

};

template<class T>
T StringUtils::filterValue(const std::string& rawValue,
                           const std::unordered_map<std::string, T>& allowedValues, T defaultValue) {
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
