#pragma once

#include <string>
#include <vector>
#include <functional>
#include <optional>

class ContainersUtils {
 public:
  ContainersUtils() = delete;

  template<class T>
  static bool contains(const std::vector<T>& container, const T& entry)
  {
    return std::find(container.begin(), container.end(), entry) != container.end();
  }

  template<class T, class Predicate>
  static bool contains(const std::vector<T>& container, const Predicate& predicate)
  {
    return std::find_if(container.begin(), container.end(), predicate) != container.end();
  }

  template<class T, class Predicate>
  static std::optional<T> find_or(const std::vector<T>& container, const Predicate& predicate)
  {
    auto foundIt = std::find_if(container.begin(), container.end(), predicate);

    if (foundIt != container.end()) {
      return *foundIt;
    }
    else {
      return std::optional<T>{};
    }
  }

  template<class T, class Predicate>
  static T filter(const T& container, Predicate predicate)
  {
    T result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), predicate);

    return result;
  }

  template<class T>
  static void append(T& target, const T& source)
  {
    target.insert(std::end(target), std::begin(source), std::end(source));
  }

  template<class SourceFormat, class TargetFormat = uint8_t>
  static std::vector<TargetFormat> convertVectorBinaryData(const std::vector<SourceFormat>& vector)
  {
    static_assert(sizeof(SourceFormat) >= sizeof(TargetFormat) &&
      sizeof(SourceFormat) % sizeof(TargetFormat) == 0);

    std::vector<TargetFormat> bytesVector(vector.size() * (sizeof(SourceFormat) / sizeof(TargetFormat)));
    memcpy_s(bytesVector.data(),
      bytesVector.size() * sizeof(bytesVector[0]),
      vector.data(),
      bytesVector.size() * sizeof(bytesVector[0]));

    return bytesVector;
  }
};

