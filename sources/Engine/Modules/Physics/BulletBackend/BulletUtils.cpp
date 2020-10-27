#include "precompiled.h"

#pragma hdrstop

#include "BulletUtils.h"

#include "Modules/Physics/CollisionShapes.h"
#include "BulletCollisionShapes.h"

btCollisionShape* BulletUtils::convertCollisionShapeToBulletShape(const CollisionShape& shape)
{
  return convertCollisionShapeToBulletShape(shape.getShapeData());
}


btCollisionShape* BulletUtils::convertCollisionShapeToBulletShape(const CollisionShapeData& shape)
{
  if (auto sphereShape = std::get_if<CollisionShapeSphere>(&shape)) {
    return new BulletSphereShape(sphereShape->getRadius());
  }
  else if (auto boxShape = std::get_if<CollisionShapeBox>(&shape)) {
    glm::vec3 boxExtents = boxShape->getHalfExtents();

    return new BulletBoxShape({boxExtents.x, boxExtents.y, boxExtents.z});
  }
  else if (auto capsuleShape = std::get_if<CollisionShapeCapsule>(&shape)) {
    return new BulletCapsuleShape(capsuleShape->getRadius(), capsuleShape->getHeight());
  }
  else if (auto triangleMeshShape = std::get_if<CollisionShapeTriangleMesh>(&shape)) {
    // TODO: optimize it, use indices instead of duplicated vertices

    auto* btTriangleMeshShape = new btTriangleMesh();

    const auto& vertices = triangleMeshShape->getVertices();

    for (size_t vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex += 3) {
      btTriangleMeshShape->addTriangle(glmVec3ToBt(vertices[vertexIndex]),
        glmVec3ToBt(vertices[vertexIndex + 1]),
        glmVec3ToBt(vertices[vertexIndex + 2]));
    }

    return new BulletBVHTriangleMeshShape(btTriangleMeshShape, true);
  }
  else if (auto compoundShape = std::get_if<CollisionShapeCompound>(&shape)) {
    auto* btCompound = new BulletCompoundShape();

    for (const auto& childShape : compoundShape->getChildren()) {
      btTransform transform;
      transform.setIdentity();

      transform.setOrigin(glmVec3ToBt(childShape.origin));

      btCompound->addChildShape(transform, convertCollisionShapeToBulletShape(childShape.shape));
    }

    return btCompound;
  }
  else {
    SW_ASSERT(false);
  }

  return nullptr;
}