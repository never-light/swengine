#include "precompiled.h"

#pragma hdrstop

#include "InputDataArchive.h"
#include "Modules/ResourceManagement/ResourcesManager.h"

InputDataArchive::InputDataArchive(std::istream& stream, std::shared_ptr<ResourcesManager> resourcesManager)
  : cereal::XMLInputArchive(stream),
    m_resourcesManager(resourcesManager)
{

}

ResourcesManager& InputDataArchive::getResourcesManager()
{
  return *m_resourcesManager;
}
