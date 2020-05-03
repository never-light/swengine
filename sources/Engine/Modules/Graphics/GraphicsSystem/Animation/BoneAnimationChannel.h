#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * @brief Represents one position keyframe in some animation channel
 */
struct BoneAnimationPositionFrame {
  float time;
  glm::vec3 position;
};

/**
 * @brief Represents one orientation keyframe in some animation channel
 */
struct BoneAnimationOrientationFrame {
  float time;
  glm::quat orientation;
};

/**
 * @brief An animation channel for a bone (list of position and orientation keyframes)
 */
class BoneAnimationChannel {
 public:
  /**
   * @brief Constructor
   */
  BoneAnimationChannel() = default;

  /**
   * @brief Constructor
   *
   * @param positionFrames List of position keyframes
   * @param orientationFrames List of orientation keyframes
   */
  BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
    const std::vector<BoneAnimationOrientationFrame>& orientationFrames);

  /**
   * @brief Destructor
   */
  ~BoneAnimationChannel() = default;

  /**
   * @brief Gets the list of position keyframes
   * @return The list of keyframes
   */
  [[nodiscard]] std::vector<BoneAnimationPositionFrame>& getPositionFrames();

  /**
   * @brief Gets the list of position keyframes
   * @return The list of keyframes
   */
  [[nodiscard]] const std::vector<BoneAnimationPositionFrame>& getPositionFrames() const;

  /**
  * @brief Gets the list of orientation keyframes
  * @return The list of keyframes
  */
  [[nodiscard]] const std::vector<BoneAnimationOrientationFrame>& getOrientationFrames() const;

  /**
  * @brief Gets the list of orientation keyframes
  * @return The list of keyframes
  */
  [[nodiscard]] std::vector<BoneAnimationOrientationFrame>& getOrientationFrames();

 private:
  /**
   * @brief The list of position keyframes in the channel
   */
  std::vector<BoneAnimationPositionFrame> m_positionFrames;

  /**
   * @brief The list of orientation keyframes in the channel
   */
  std::vector<BoneAnimationOrientationFrame> m_orientationFrames;
};
