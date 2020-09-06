#pragma once

#include <btBulletCollisionCommon.h>

class BulletSphereShape : public btSphereShape {
 public:
  explicit BulletSphereShape(btScalar radius) : btSphereShape(radius)
  {

  }
};

class BulletBoxShape : public btBoxShape {
 public:
  explicit BulletBoxShape(const btVector3& boxHalfExtents) : btBoxShape(boxHalfExtents)
  {

  }
};

class BulletCapsuleShape : public btCapsuleShape {
 public:
  BulletCapsuleShape(btScalar radius, btScalar height) : btCapsuleShape(radius, height)
  {

  }
};

class BulletBVHTriangleMeshShape : public btBvhTriangleMeshShape {
 public:
  BulletBVHTriangleMeshShape(btStridingMeshInterface* meshInterface,
    bool useQuantizedAabbCompression,
    bool buildBvh = true)
    : btBvhTriangleMeshShape(meshInterface, useQuantizedAabbCompression, buildBvh)
  {

  }
};

class BulletCompoundShape : public btCompoundShape {
 public:
  explicit BulletCompoundShape(bool enableDynamicAabbTree = true, int initialChildCapacity = 0)
    : btCompoundShape(enableDynamicAabbTree, initialChildCapacity)
  {

  }
};