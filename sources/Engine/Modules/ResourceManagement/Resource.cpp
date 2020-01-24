#include "precompiled.h"
#pragma hdrstop

#include "Resource.h"


Resource::Resource()
{

}

Resource::~Resource()
{

}

bool Resource::isLoaded() const
{
    return m_loaded;
}

bool Resource::isPersistent() const
{
    return m_persistent;
}

void Resource::setPersistent(bool persistent)
{
    m_persistent = persistent;
}

void Resource::performLoad(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
    SW_ASSERT(!isLoaded());

    load(declaration, resourceManager);
    m_loaded = true;
}

void Resource::performUnload()
{
    SW_ASSERT(isLoaded() && !isBusy() && !isPersistent());

    unload();
    m_loaded = false;
}

void Resource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
    ARG_UNUSED(declaration);
    ARG_UNUSED(resourceManager);
}

void Resource::unload()
{

}
