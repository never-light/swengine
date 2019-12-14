#pragma once

#include <memory>
#include <variant>
#include <any>
#include <string>
#include <vector>

#include "Exceptions/EngineRuntimeException.h"
#include "Utility/strings.h"
#include "Utility/xml.h"

struct ResourceSourceFile {
    std::string path;
};

struct ResourceSourceFilesList {
    std::vector<std::string> paths;
};

struct ResourceSourceRawString {
    std::string data;
};

struct ResourceSourceParameters {

};

using ResourceSource = std::variant<ResourceSourceFile, ResourceSourceRawString>;

struct ResourceDeclaration {
    ResourceSource source;
    std::any parameters;

    template<class T>
    T getParameters() const;
};

template<class T>
T ResourceDeclaration::getParameters() const
{
    static_assert (std::is_base_of_v<ResourceSourceParameters, T>);

    try {
        return std::any_cast<T>(parameters);
    }
    catch(const std::bad_any_cast&) {
        ENGINE_RUNTIME_ERROR("Trying to get resource source parameters with invalid type");
    }
}

class ResourceInstance;

class Resource
{
public:
    Resource();
    virtual ~Resource();

    virtual void load(const ResourceDeclaration& declaration) = 0;
    virtual void unload() = 0;

    virtual bool isBusy() const = 0;

    bool isLoaded() const;
    bool isPersistent() const;

    void setPersistent(bool persistent);

private:
    void performLoad(const ResourceDeclaration& declaration);
    void performUnload();

private:
    bool m_loaded = false;
    bool m_persistent = false;

private:
    friend class ResourceInstance;
};

class ResourceDeclHelpers {
public:
    template<class T>
    static T getFilteredParameterValue(const pugi::xml_node& declarationNode, const std::string& parameterName,
                                        const std::unordered_map<std::string, T>& allowedValues, T defaultValue);

    template<class T>
    static T getFilteredParameterValue(const std::string& rawValue, const std::string& parameterName,
                                        const std::unordered_map<std::string, T>& allowedValues, T defaultValue);
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
        ENGINE_RUNTIME_ERROR("Resource parameter has invalid value (" + parameterName + "=" + rawValue +  ")");
    }
}

