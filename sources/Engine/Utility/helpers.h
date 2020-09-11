#pragma once

#ifdef _MSC_VER
#define DISABLE_WARNINGS()             \
  __pragma(warning(push, 0))           \
  __pragma(warning (disable : 4701))

#define ENABLE_WARNINGS() __pragma(warning(pop))
#else
#error "Declare ENABLE_WARNINGS and DISABLE_WARNINGS macros for the current compiler"
#endif
