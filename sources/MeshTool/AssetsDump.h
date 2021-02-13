#pragma once

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include "RawScene.h"

inline std::ostream& operator<<(std::ostream& stream, const RawVector2& v)
{
  stream << "{" << v.x << ", " << v.y << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawVector3& v)
{
  stream << "{" << v.x << ", " << v.y << ", " << v.z << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawVector4& v)
{
  stream << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawQuaternion& v)
{
  stream << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawU8Vector4& v)
{
  stream << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawMatrix4& m)
{
  stream << glm::to_string(rawMatrix4ToGLMMatrix4(m));
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const uint16_t& v)
{
  stream << static_cast<int>(v);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawBonePositionFrame& v)
{
  stream << "{" << v.time << ", " << v.position << "}";
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const RawBoneOrientationFrame& v)
{
  stream << "{" << v.time << ", " << v.orientation << "}";
  return stream;
}

class AssetsDump {
 public:
  AssetsDump() = default;

  void dumpAssetData(const std::string& assetPath);

 private:
  static void dumpMesh(const RawMesh& mesh);
  static void dumpSkeleton(const RawSkeleton& skeleton);
  static void dumpAnimationClip(const RawSkeletalAnimationClip& animationClip);

 private:
  template<class T>
  static void dumpVectorSection(const std::string& sectionName, const std::vector<T>& vector)
  {
    std::stringstream valuesStream;

    size_t valueIndex = 0;

    for (const auto& value : vector) {
      valuesStream << "#(" << valueIndex << ")" << value << ", ";
      valueIndex++;
    }

    std::string valuesRepresentation = valuesStream.str().substr(0, valuesStream.str().size() - 2);

    fmt::print("{}:\n{}\n", sectionName, valuesRepresentation);
  }
};
