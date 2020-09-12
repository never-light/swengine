#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsLoader.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/Resources/MaterialResource.h"

#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Physics/Resources/CollisionDataResource.h"

#include "Modules/Math/MathUtils.h"
#include "Utility/files.h"

GameObjectsLoader::GameObjectsLoader(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_resourceManager(std::move(resourceManager))
{
  registerGenericComponentLoader("transform",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadTransformData(object, objectNode);
    });

  registerGenericComponentLoader("visual",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadVisualData(object, objectNode);
    });

  registerGenericComponentLoader("rigid_body",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadRigidBodyData(object, objectNode);
    });
}

GameObjectsLoader::~GameObjectsLoader() = default;

void GameObjectsLoader::registerClassLoader(const std::string& className,
  std::unique_ptr<GameObjectsClassLoader> loader)
{
  SW_ASSERT(!m_classesLoaders.contains(className));

  m_classesLoaders.insert({className, std::move(loader)});
}

void GameObjectsLoader::loadTransformData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto& transformComponent = gameObject.addComponent<TransformComponent>();

  bool isStatic = data.attribute("static").as_bool(false);
  transformComponent.setStaticMode(isStatic);

  auto positionAttr = data.attribute("position");

  if (positionAttr) {
    transformComponent.getTransform().setPosition(StringUtils::stringToVec3(positionAttr.as_string()));
  }

  auto directionAttr = data.attribute("direction");

  if (directionAttr) {
    glm::vec3 forwardDirection = StringUtils::stringToVec3(directionAttr.as_string());

    transformComponent.getTransform().setOrientation(
      MathUtils::forwardUpToQuat(forwardDirection, MathUtils::AXIS_UP));
  }

  auto scaleAttr = data.attribute("scale");

  if (scaleAttr) {
    transformComponent.getTransform().setScale(StringUtils::stringToVec3(scaleAttr.as_string()));
  }
}

void GameObjectsLoader::loadVisualData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto& transformComponent = gameObject.getComponent<TransformComponent>();

  auto meshName = data.attribute("mesh").as_string();

  std::shared_ptr<Mesh> meshInstance =
    m_resourceManager->getResourceFromInstance<MeshResource>(meshName)->getMesh();

  auto& meshRendererComponent = gameObject.addComponent<MeshRendererComponent>();
  // TODO: remove isStatic option and use isStatic from TransformComponent
  meshRendererComponent.getAttributes().isStatic = true;
  meshRendererComponent.setMeshInstance(meshInstance);
  meshRendererComponent.updateBounds(transformComponent.getTransform().getTransformationMatrix());

  auto materialsData = data.child("materials");

  for (auto materialData : materialsData.children("material")) {
    auto materialName = materialData.attribute("id").as_string();
    int materialSubMeshIndex = materialData.attribute("index").as_int();

    std::shared_ptr<Material> materialInstance =
      m_resourceManager->getResourceFromInstance<MaterialResource>(materialName)->getMaterial();

    meshRendererComponent.setMaterialInstance(materialSubMeshIndex, materialInstance);
  }
}

void GameObjectsLoader::loadRigidBodyData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto collisionModelName = data.attribute("collision_model").as_string();

  std::shared_ptr<CollisionShape> collisionShape =
    m_resourceManager->getResourceFromInstance<CollisionDataResource>(collisionModelName)->getCollisionShape();

  // Set zero mass to mark rigid body as static body
  auto& rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(0.0f, collisionShape);
  LOCAL_VALUE_UNUSED(rigidBodyComponent);
}

void GameObjectsLoader::registerGenericComponentLoader(const std::string& componentName,
  const GameObjectsLoader::ComponentLoaderCallback& loader)
{
  SW_ASSERT(!m_componentsLoaders.contains(componentName));

  m_componentsLoaders.insert({componentName, loader});
}

GameObjectsLoader::ComponentLoaderCallback GameObjectsLoader::getComponentLoader(const std::string& componentName) const
{
  return m_componentsLoaders.at(componentName);
}

ResourceManager& GameObjectsLoader::getResourceManager()
{
  return *m_resourceManager;
}

GameObject& GameObjectsLoader::loadGameObject(const pugi::xml_node& objectNode)
{
  std::string objectClassName = objectNode.attribute("class").as_string();
  auto& classLoader = m_classesLoaders.at(objectClassName);

  auto gameObject = m_gameWorld->createGameObject();

  classLoader->loadGameObject(*gameObject, objectNode);

  return *gameObject;
}
