#pragma once

#include <memory>
#include <variant>
#include <any>
#include <string>

#include "Exceptions/EngineRuntimeException.h"

struct ResourceSourceFile {
    std::string path;
};

struct ResourceSourceRawString {
    std::string data;
};

struct ResourceSourceParameters {

};

struct ResourceSource {
    std::variant<ResourceSourceFile, ResourceSourceRawString> source;
    std::any parameters;

    template<class T>
    T getParameters() const;
};

template<class T>
T ResourceSource::getParameters() const
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

    virtual void load(const ResourceSource& source) = 0;
    virtual void unload() = 0;

    virtual bool isBusy() const = 0;

    bool isLoaded() const;
    bool isPersistent() const;

    void setPersistent(bool persistent);

private:
    void performLoad(const ResourceSource& source);
    void performUnload();

private:
    bool m_loaded = false;
    bool m_persistent = false;

private:
    friend class ResourceInstance;
};
