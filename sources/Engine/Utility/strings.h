#pragma once

#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <spdlog/spdlog.h>

#include "Exceptions/EngineRuntimeException.h"

class StringUtils {
public:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string toLowerCase(const std::string& str);

    template<class T>
    static T filterValue(const std::string& rawValue,
        const std::unordered_map<std::string, T>& allowedValues, T defaultValue);

    static std::string regexReplace(const std::string& expression, std::string str,
        std::function<std::string(const std::smatch&)> callback);

    static std::string replace(std::string source,
                               const std::string& pattern,
                               const std::string& replacement);

private:
    StringUtils() = delete;
};

template<class T>
T StringUtils::filterValue(const std::string& rawValue,
    const std::unordered_map<std::string, T>& allowedValues, T defaultValue)
{
    T value = defaultValue;

    if (!rawValue.empty()) {
        auto valueIt = allowedValues.find(rawValue);

        if (valueIt == allowedValues.end()) {
             ENGINE_RUNTIME_ERROR("The value has invalid format");
        }

        value = valueIt->second;
    }

    return value;
}
