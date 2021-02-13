#pragma once

#include <cassert>

#define ENGINE_VERSION "0.0.1"

#define ARG_UNUSED(arg) (void)(arg)
#define RETURN_VALUE_UNUSED(value) (void)(value)
#define LOCAL_VALUE_UNUSED(value) (void)(value)

#ifdef _DEBUG
inline void swAssert(bool condition) {
  if (!condition) {
    assert(false);
  }
}

#define SW_ASSERT(condition) swAssert(condition)
#else
#define SW_ASSERT(condition) ((void)(condition))
#endif

#define SW_STATIC_ASSERT static_assert

#ifdef _DEBUG
#ifdef _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#else
#warning "Debug break feature is not available on Linux platform"
#define DEBUG_BREAK()
#endif
#else
#define DEBUG_BREAK()
#endif