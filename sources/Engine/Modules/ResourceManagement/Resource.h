#pragma once

#include <memory>

enum class ResourceType {
    Mesh, Texture, GpuProgram
};

class Resource
{
public:
    Resource();
    virtual ~Resource();
};

using ResourceSharedPtr = std::shared_ptr<Resource>;
