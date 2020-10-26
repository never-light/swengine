#pragma once

#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Bone.h"

/**
 * @brief The call represents collection of bones for skeletal animation of skinned mesh
 */
class Skeleton : public Resource {
 public:
  /**
   * @brief Constructor
   * @param bones A list of bones
   */
  explicit Skeleton(const std::vector<Bone>& bones);

  /**
   * @brief Copy constructor
   * @param skeleton Skeleton to copy
   */
  Skeleton(const Skeleton& skeleton) = default;

  /**
   * @brief Destructor
   */
  ~Skeleton() override = default;

  /**
   * @brief Returns the root bone
   * @remarks The root bone always has id ROOT_BONE_PARENT_ID
   * @return The root bone
   */
  [[nodiscard]] const Bone& getRootBone() const;

  /**
   * @brief Returns bones count in the skeleton
   * @return Bones count
   */
  [[nodiscard]] uint8_t getBonesCount() const;

  /**
   * @brief Returns bone by its id
   * @param id Bone id
   * @return Bone object
   */
  [[nodiscard]] const Bone& getBone(uint8_t id) const;

  /**
   * @brief Returns bone's parent id by bone id
   * @param id Bone id
   * @return Bone parent id
   */
  [[nodiscard]] uint8_t getBoneParentId(uint8_t id) const;

 private:
  /**
   * @brief A list of bones
   */
  std::vector<Bone> m_bones;
};

