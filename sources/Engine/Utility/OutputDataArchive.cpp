#include "precompiled.h"

#pragma hdrstop

#include "OutputDataArchive.h"
#include "Modules/ResourceManagement/ResourcesManager.h"

OutputDataArchive::OutputDataArchive(std::ostream& stream, std::shared_ptr<ResourcesManager> resourcesManager)
  : cereal::XMLOutputArchive(stream),
  m_resourcesManager(resourcesManager)
{

}

ResourcesManager& OutputDataArchive::getResourcesManager()
{
  return *m_resourcesManager;
}
