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
};

