#include "precompiled.h"

#pragma hdrstop

#include "CollisionShapeResourceManager.h"
#include "Exceptions/exceptions.h"

#include "Modules/Math/MathUtils.h"

#include "Modules/Physics/Resources/Raw/RawMeshCollisionData.h"
#include "Modules/Physics/CollisionShapesFactory.h"

#include "Utility/memory.h"

CollisionShapeResourceManager::CollisionShapeResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<CollisionShape, CollisionShapeResourceConfig>(resourcesManager)
{

}

CollisionShapeResourceManager::~CollisionShapeResourceManager() = default;

void CollisionShapeResourceManager::load(size_t resourceIndex)
{
  CollisionShapeResourceConfig* config = getResourceConfig(resourceIndex);

  // Read raw mesh
  RawMeshCollisionData rawCollisionData = RawMeshCollisionData::readFromFile(config->resourcePath);

  // Convert raw collision shapes to internal collision shapes objects
  std::vector<CollisionShapeCompoundChild> collisionShapes;

  for (const auto& rawShape : rawCollisionData.collisionShapes) {
    switch (rawShape.type) {
      case RawMeshCollisionShapeType::AABB: {
        glm::vec3 min = {rawShape.aabb.min.x, rawShape.aabb.min.y, rawShape.aabb.min.z};
        glm::vec3 max = {rawShape.aabb.max.x, rawShape.aabb.max.y, rawShape.aabb.max.z};

        collisionShapes.push_back({CollisionShapeBox((max - min) / 2.0f), (max + min) / 2.0f});

        break;
      }

      case RawMeshCollisionShapeType::Sphere: {
        collisionShapes.push_back({CollisionShapeSphere(rawShape.sphere.radius),
          {rawShape.sphere.origin.x, rawShape.sphere.origin.y, rawShape.sphere.origin.z}});

        break;
      }

      case RawMeshCollisionShapeType::TriangleMesh: {
        std::vector<glm::vec3> vertices =
          MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawShape.triangleMesh.vertices);

        collisionShapes.push_back({CollisionShapeTriangleMesh(vertices), {0.0f, 0.0f, 0.0f}});

        break;
      }

      default:
        SW_ASSERT(false);
        break;
    }
  }

  CollisionShape collisionShape;

  SW_ASSERT(!collisionShapes.empty());

  if (collisionShapes.size() == 1) {
    // Assume that if the mesh has only one collider, it should be located in center of local coordinate system
    SW_ASSERT(MathUtils::isEqual(collisionShapes.begin()->origin, {0.0f, 0.0f, 0.0f}, 1e-2f));

    collisionShape = CollisionShape(collisionShapes.begin()->shape);
  }
  else {
    collisionShape = CollisionShape(CollisionShapeCompound(collisionShapes));
  }

  allocateResource<CollisionShape>(resourceIndex, collisionShape);
}

void CollisionShapeResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  CollisionShapeResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Collision shape resource refer to not existing file", resourceConfig->resourcePath));
  }
}
