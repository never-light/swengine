#include "precompiled.h"

#pragma hdrstop

#include "CollisionDataResource.h"
#include "Exceptions/exceptions.h"

#include "Modules/Math/MathUtils.h"

#include "Modules/Physics/Resources/Raw/RawMeshCollisionData.h"
#include "Modules/Physics/CollisionShapesFactory.h"

#include "Utility/memory.h"

CollisionDataResource::CollisionDataResource() = default;

CollisionDataResource::~CollisionDataResource()
{
  SW_ASSERT(m_collisionShape.use_count() <= 1);
}

void CollisionDataResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
  ARG_UNUSED(resourceManager);

  SW_ASSERT(m_collisionShape == nullptr);

  auto parameters = declaration.getParameters<CollisionDataResourceParameters>();

  if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
    m_collisionShape = loadFromFile(sourceFile->path, parameters);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load collision data resource from invalid source");
  }
}

void CollisionDataResource::unload()
{
  SW_ASSERT(m_collisionShape.use_count() == 1);

  m_collisionShape.reset();
}

bool CollisionDataResource::isBusy() const
{
  return m_collisionShape.use_count() > 1;
}

std::shared_ptr<CollisionShape> CollisionDataResource::loadFromFile(const std::string& path,
  const CollisionDataResourceParameters& parameters)
{
  ARG_UNUSED(parameters);

  // Read raw mesh
  RawMeshCollisionData rawCollisionData = RawMeshCollisionData::readFromFile(path);

  // Convert raw collision shapes to internal collision shapes objects
  std::vector<CollisionShapeCompoundChild> collisionShapes;

  for (const auto& rawShape : rawCollisionData.collisionShapes) {
    CollisionShapeCompoundChild childShape;

    switch (rawShape.type) {
      case RawMeshCollisionShapeType::AABB: {
        glm::vec3 min = {rawShape.aabb.min.x, rawShape.aabb.min.y, rawShape.aabb.min.z};
        glm::vec3 max = {rawShape.aabb.max.x, rawShape.aabb.max.y, rawShape.aabb.max.z};

        childShape.shape = std::make_shared<CollisionShapeBox>((max - min) / 2.0f);
        childShape.origin = (max + min) / 2.0f;

        break;
      }

      case RawMeshCollisionShapeType::Sphere: {
        childShape.shape = std::make_shared<CollisionShapeSphere>(rawShape.sphere.radius);
        childShape.origin = {rawShape.sphere.origin.x, rawShape.sphere.origin.y, rawShape.sphere.origin.z};

        break;
      }

      case RawMeshCollisionShapeType::TriangleMesh: {
        std::vector<glm::vec3> vertices =
          MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawShape.triangleMesh.vertices);

        childShape.shape = std::make_shared<CollisionShapeTriangleMesh>(vertices);
        childShape.origin = {0.0f, 0.0f, 0.0f};

        break;
      }

      default:
        SW_ASSERT(false);
        break;
    }

    collisionShapes.push_back(childShape);
  }

  std::shared_ptr<CollisionShape> collisionShape;

  SW_ASSERT(!collisionShapes.empty());

  if (collisionShapes.size() == 1) {
    // Assume that if the mesh has only one collider, it should be located in center of local coordinate system
    SW_ASSERT(MathUtils::isEqual(collisionShapes.begin()->origin, { 0.0f, 0.0f, 0.0f }, 1e-2f));

    collisionShape = collisionShapes.begin()->shape;
  }
  else {
    collisionShape = std::make_shared<CollisionShapeCompound>(collisionShapes);
  }

  return collisionShape;
}

CollisionDataResource::ParametersType CollisionDataResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
  const ParametersType& defaultParameters)
{
  ARG_UNUSED(declarationNode);

  ParametersType parameters = defaultParameters;

  return parameters;
}

std::shared_ptr<CollisionShape> CollisionDataResource::getCollisionShape() const
{
  return m_collisionShape;
}
