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

void Resource::performLoad(const ResourceSource &source)
{
    SW_ASSERT(!isLoaded());

    load(source);
    m_loaded = true;
}

void Resource::performUnload()
{
    SW_ASSERT(isLoaded() && !isBusy() && !isPersistent());

    unload();
    m_loaded = false;
}

void Resource::load(const ResourceSource &source)
{
    ARG_UNUSED(source);
}

void Resource::unload()
{

}
