#pragma once

#include <string>
#include <random>
#include <concepts>
#include <type_traits>

#include "swdebug.h"

class Random {
 public:
  Random() = delete;

  template<class T>
  static inline T get(T min, T max)
  {
    SW_ASSERT(false && "Use another type or add appropriate template specialization");
  }

  template<class T>
  static inline T get(const std::vector<T>& container)
  {
    return container[get<size_t>(0, container.size() - 1)];
  }

  static std::mt19937& getMt19937();
};

template<>
inline int Random::get(int min, int max)
{
  std::uniform_int_distribution<int> dist(min, max);
  return dist(getMt19937());
}

template<>
inline size_t Random::get(size_t min, size_t max)
{
  std::uniform_int_distribution<size_t> dist(min, max);
  return dist(getMt19937());
}

template<>
inline float Random::get(float min, float max)
{
  std::uniform_real_distribution<float> dist(min, std::nextafter(max, std::numeric_limits<float>::max()));
  return dist(getMt19937());
}
