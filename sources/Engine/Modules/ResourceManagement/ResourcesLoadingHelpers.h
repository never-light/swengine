#pragma once

#include <memory>
#include <variant>
#include <any>
#include <string>
#include <vector>

#include "Exceptions/exceptions.h"
#include "Utility/strings.h"
#include "Utility/xml.h"


class ResourceDeclHelpers {
 public:
  template<class T>
  [[nodiscard]] static T getFilteredParameterValue(const pugi::xml_node& declarationNode,
    const std::string& parameterName,
    const std::unordered_map<std::string, T>& allowedValues,
    T defaultValue);

  template<class T>
  [[nodiscard]] static T getFilteredParameterValue(const std::string& rawValue,
    const std::string& parameterName,
    const std::unordered_map<std::string, T>& allowedValues,
    T defaultValue);
};

template<class T>
T ResourceDeclHelpers::getFilteredParameterValue(const pugi::xml_node& declarationNode,
  const std::string& parameterName,
  const std::unordered_map<std::string, T>& allowedValues,
  T defaultValue)
{
  std::string rawValue = declarationNode.child_value(parameterName.c_str());

  return ResourceDeclHelpers::getFilteredParameterValue(rawValue, parameterName, allowedValues, defaultValue);
}

template<class T>
T ResourceDeclHelpers::getFilteredParameterValue(const std::string& rawValue,
  const std::string& parameterName,
  const std::unordered_map<std::string, T>& allowedValues,
  T defaultValue)
{
  try {
    return StringUtils::filterValue(StringUtils::toLowerCase(rawValue), allowedValues, defaultValue);
  }
  catch (const EngineRuntimeException&) {
    THROW_EXCEPTION(EngineRuntimeException,
      "Resource parameter has invalid value (" + parameterName + "=" + rawValue + ")");
  }
}

