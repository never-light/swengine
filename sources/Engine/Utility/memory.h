#pragma once

#include <vector>
#include <memory>
#include <type_traits>

class MemoryUtils {
 public:
  template<class SourceType, class TargetType>
  [[nodiscard]] static std::vector<TargetType> createBinaryCompatibleVector(std::vector<SourceType>& source);
};

template<class SourceType, class TargetType>
std::vector<TargetType> MemoryUtils::createBinaryCompatibleVector(std::vector<SourceType>& source)
{
  static_assert(std::is_pod_v<SourceType> && std::is_pod_v<TargetType> &&
    sizeof(SourceType) == sizeof(TargetType));

  size_t dataSize = sizeof(SourceType);

  std::vector<TargetType> target;

  if (source.size() > 0) {
    target.resize(source.size());
    memcpy_s(target.data(), source.size() * dataSize, source.data(), source.size() * dataSize);
  }

  return target;
}

