#include "InfoportionsSystem.h"

#include <utility>
#include <Engine/swdebug.h>
#include <Engine/Utility/files.h>
#include <Engine/Utility/xml.h>

#include "ActorComponent.h"

ToggleInfoportionEvent::ToggleInfoportionEvent(GameObject actor, std::string infoportionName)
  : m_actor(actor),
    m_infoportionName(std::move(infoportionName))
{

}

GameObject ToggleInfoportionEvent::getActor() const
{
  return m_actor;
}

const std::string& ToggleInfoportionEvent::getInfoportionName() const
{
  return m_infoportionName;
}

AddInfoportionCommandEvent::AddInfoportionCommandEvent(GameObject actor, std::string infoportionName)
  : ToggleInfoportionEvent(actor, std::move(infoportionName))
{

}

RemoveInfoportionCommandEvent::RemoveInfoportionCommandEvent(GameObject actor, std::string infoportionName)
  : ToggleInfoportionEvent(actor, std::move(infoportionName))
{

}

AddInfoportionEvent::AddInfoportionEvent(GameObject actor, std::string infoportionName)
  : ToggleInfoportionEvent(actor, std::move(infoportionName))
{

}

RemoveInfoportionEvent::RemoveInfoportionEvent(GameObject actor, std::string infoportionName)
  : ToggleInfoportionEvent(actor, std::move(infoportionName))
{

}

void ActorInfoportionsStorage::addInfoportion(const std::string& infoportion)
{
  SW_ASSERT(!hasInfoportion(infoportion));
  m_storage.insert(infoportion);
}

void ActorInfoportionsStorage::removeInfoportion(const std::string& infoportion)
{
  SW_ASSERT(hasInfoportion(infoportion));

  m_storage.erase(infoportion);
}

bool ActorInfoportionsStorage::hasInfoportion(const std::string& infoportion) const
{
  return m_storage.contains(infoportion);
}

void InfoportionsSystem::addInfoportion(const std::string& infoportion)
{
  m_storage.insert(infoportion);
}

bool InfoportionsSystem::hasInfoportion(const std::string& infoportion) const
{
  return m_storage.contains(infoportion);
}

void InfoportionsSystem::loadInfoportionsFromFile(const std::string& path)
{
  std::string infoportionsDescriptionPath = FileUtils::getGameResourcePath(
    "quests/" + path + ".xml");

  spdlog::debug("Load infoportions list from {}", infoportionsDescriptionPath);

  auto infoportionsDescription = XMLUtils::openDescriptionFile(
    infoportionsDescriptionPath, "infoportions");

  pugi::xml_node infoportionsListNode = std::get<1>(infoportionsDescription);

  for (pugi::xml_node infoportionNode : infoportionsListNode.children("infoportion")) {
    std::string infoportionId = infoportionNode.attribute("id").as_string();

    addInfoportion(infoportionId);
  }
}

void InfoportionsSystem::activate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->subscribeEventsListener<AddInfoportionCommandEvent>(this);
  gameWorld->subscribeEventsListener<RemoveInfoportionCommandEvent>(this);
}

void InfoportionsSystem::deactivate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<RemoveInfoportionCommandEvent>(this);
  gameWorld->unsubscribeEventsListener<AddInfoportionCommandEvent>(this);
}

EventProcessStatus InfoportionsSystem::receiveEvent(const AddInfoportionCommandEvent& event)
{
  SW_ASSERT(hasInfoportion(event.getInfoportionName()));

  GameObject targetActor = event.getActor();
  auto& targetActorInfoportionsStorage = targetActor.getComponent<ActorComponent>()->
    getInfoportionsStorage();

  if (!targetActorInfoportionsStorage.hasInfoportion(event.getInfoportionName())) {
    targetActorInfoportionsStorage.addInfoportion(event.getInfoportionName());

    getGameWorld()->emitEvent(AddInfoportionEvent(targetActor, event.getInfoportionName()));
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus InfoportionsSystem::receiveEvent(const RemoveInfoportionCommandEvent& event)
{
  SW_ASSERT(hasInfoportion(event.getInfoportionName()));

  GameObject targetActor = event.getActor();
  auto& targetActorInfoportionsStorage = targetActor.getComponent<ActorComponent>()->
    getInfoportionsStorage();

  if (targetActorInfoportionsStorage.hasInfoportion(event.getInfoportionName())) {
    targetActorInfoportionsStorage.removeInfoportion(event.getInfoportionName());

    getGameWorld()->emitEvent(RemoveInfoportionEvent(targetActor, event.getInfoportionName()));
  }

  return EventProcessStatus::Processed;
}
