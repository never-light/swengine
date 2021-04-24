#pragma once

#include <type_traits>

template<class ...Ts>
struct EmptyType {
  using type = void;
};

#define DECLARE_VARIABLE_EXISTENCE_CHECKER(checkerName, variableName) \
  template<typename T, typename = int>                                \
   struct checkerName : std::false_type {};                           \
                                                                      \
  template<typename T>                                                \
  struct checkerName<T,                                               \
    decltype((void)T::variableName, 0)> : std::true_type {};

#define DECLARE_METHOD_EXISTENCE_CHECKER(checkerName, methodName)     \
  template<typename T, class = void>                                  \
   struct checkerName : std::false_type {};                           \
                                                                      \
  template<class T>                                                   \
  struct checkerName<T, typename EmptyType<decltype(                  \
    std::declval<T>().methodName())>::type> : std::true_type {};
