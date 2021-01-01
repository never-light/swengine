#pragma once

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

using TracedExceptionWrapper = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

#define GENERIC_ERROR(error, message) boost::enable_error_info( \
  EngineRuntimeException(message, __FILE__, __LINE__, __FUNCTION__)) << TracedExceptionWrapper(boost::stacktrace::stacktrace());

#define THROW_EXCEPTION(exception, message) do { \
    DEBUG_BREAK(); \
    throw GENERIC_ERROR(exception, message); \
  } while(true)

// TODO: replace THROW_EXCEPTION with specific macros
