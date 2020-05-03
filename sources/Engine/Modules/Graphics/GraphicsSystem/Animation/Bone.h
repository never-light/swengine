#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * @brief Represents pose of a bone relative to the its parent
 * or skinned mesh space for the root bone
 */
struct BonePose {
 public:
  /**
   * @brief Constructor
   */
  BonePose() = default;

  /**
   * @brief Constructor
   *
   * @param position A position of the bone relative to the parent position
   * @param orientation An orientation of the bone relative to the parent orientation
   */
  BonePose(const glm::vec3& position, const glm::quat& orientation);

  /**
   * @brief Returns transformation matrix for the bone
   * (conversion from bone space to parent bone space)
   *
   * @return Bone transformation matrix
   */
  [[nodiscard]] glm::mat4 getBoneMatrix() const;

 public:
  /**
   * @brief Interpolates two bone poses
   *
   * @param first The first bone pose
   * @param second The second bone pose
   * @param factor Interpolation factor (from 0.0 to 1.0)
   *
   * @return Interpolated pose
   */
  [[nodiscard]] static BonePose interpolate(const BonePose& first, const BonePose& second, float factor);

 public:
  /**
   * @brief A position of the bone relative to the parent position
   */
  glm::vec3 position = glm::vec3(0.0f);

  /**
   * @brief An orientation of the bone relative to the parent orientation
   */
  glm::quat orientation = glm::identity<glm::quat>();
};

/**
 * @brief Represents one bone of an skeleton for skinned mesh
 */
struct Bone {
 public:
  /**
   * @brief Constructor
   */
  Bone();

  /**
   * @brief Constructor
   *
   * @param name Bone name
   * @param parentId Bone parent id
   * @param inverseBindPoseMatrix Inverse bind pose matrix (\ref setInverseBindPoseMatrix)
   */
  Bone(std::string  name, uint8_t parentId, const glm::mat4& inverseBindPoseMatrix);

  /**
   * @brief Destructor
   */
  ~Bone() = default;

  /**
   * @brief Sets a name of the bone
   * @param name A new name of the bone
   */
  void setName(const std::string& name);

  /**
   * @brief Gets the name of the bone
   * @return The name of the bone
   */
  [[nodiscard]] const std::string& getName() const;

  /**
   * @brief Sets a parent id of the bone
   * @param parentId A new parent id of the bone
   */
  void setParentId(const uint8_t parentId);

  /**
   * @brief Gets the parent id of the bone
   * @return The parent id of the bone
   */
  [[nodiscard]] uint8_t getParentId() const;

  /**
   * @brief Sets an inverse bind pose matrix.
   * The matrix represents conversion from mesh bind pose space to the bone space.
   * Bind pose is a pose in which the skeleton was attached to the mesh.
   * @remarks The original bind pose matrix is a matrix for conversion from bone space to mesh space.
   *
   * @param inverseBindPoseMatrix Inverse bind pose matrix
   */
  void setInverseBindPoseMatrix(const glm::mat4& inverseBindPoseMatrix);

  /**
   * @brief Gets the inverse bind pose matrix of the bone
   *
   * @return The inverse bind pose matrix of the bone
   */
  [[nodiscard]] const glm::mat4& getInverseBindPoseMatrix() const;

 public:
  /**
   * @brief Parent id of the root bone in a skeleton
   */
  static constexpr uint8_t ROOT_BONE_PARENT_ID = 255;

 private:
  /**
   * @brief A name of the bone
   */
  std::string m_name;

  /**
   * @brief An parent id of the bone
   */
  uint8_t m_parentId;

  /**
   * @brief An inverse bind pose matrix of the bone
   */
  glm::mat4 m_inverseBindPoseMatrix;
};

/**
 * @brief Combines two poses (gets position and orientation of the first pose
 * relative to second bone parent space)
 *
 * @param a The first pose
 * @param b The second pose
 *
 * @return Combined pose
 */
inline BonePose operator*(const BonePose& a, const BonePose& b)
{
  BonePose result;
  result.orientation = a.orientation * b.orientation;
  result.position = a.position + glm::vec3(a.orientation * glm::vec4(b.position, 1.0f));

  return result;
}
