#pragma once

#include "Resource.h"

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    virtual ResourceSharedPtr load() = 0;
};

