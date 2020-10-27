#pragma once

#include <cassert>

#define ENGINE_VERSION "0.0.1"

#define ARG_UNUSED(arg) (void)(arg)
#define RETURN_VALUE_UNUSED(value) (void)(value)
#define LOCAL_VALUE_UNUSED(value) (void)(value)

#ifdef _DEBUG
#define SW_ASSERT(condition) assert(condition)
#else
#define SW_ASSERT(condition) ((void)(condition))
#endif

#define SW_STATIC_ASSERT static_assert