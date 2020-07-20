#pragma once

#include <btBulletDynamicsCommon.h>
#include "Modules/Physics/CollisionShapes.h"

struct BulletCollisionShape {
 public:
  BulletCollisionShape() = default;
  virtual ~BulletCollisionShape() = default;

  [[nodiscard]] virtual btCollisionShape* getCollisionShape() const = 0;
};

struct BulletCollisionShapeSphere : public BulletCollisionShape, public CollisionShapeSphere {
 public:
  BulletCollisionShapeSphere(float radius);
  ~BulletCollisionShapeSphere() override;

  [[nodiscard]] float getRadius() const override;

  [[nodiscard]] btCollisionShape* getCollisionShape() const override;

 private:
  btSphereShape* m_shape;
};

struct BulletCollisionShapeBox : public BulletCollisionShape, public CollisionShapeBox {
 public:
  BulletCollisionShapeBox(const glm::vec3& halfExtents);
  ~BulletCollisionShapeBox() override;

  [[nodiscard]] glm::vec3 getHalfExtents() const override;

  [[nodiscard]] btCollisionShape* getCollisionShape() const override;

 private:
  btBoxShape* m_shape;
};

struct BulletCollisionShapeCapsule : public BulletCollisionShape, public CollisionShapeCapsule {
 public:
  BulletCollisionShapeCapsule(float radius, float height);
  ~BulletCollisionShapeCapsule() override;

  [[nodiscard]] float getRadius() const override;
  [[nodiscard]] float getHeight() const override;

  [[nodiscard]] btCollisionShape* getCollisionShape() const override;

 private:
  btCapsuleShape* m_shape;
};
