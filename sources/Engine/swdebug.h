#pragma once

#include <cassert>

#define ENGINE_VERSION "0.0.1"

#define ARG_UNUSED(arg) (void)(arg)
#define RETURN_VALUE_UNUSED(value) (void)(value)
#define LOCAL_VALUE_UNUSED(value) (void)(value)

#define SW_ASSERT(condition) assert(condition)
