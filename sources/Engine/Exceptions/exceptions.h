#pragma once

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

#include "EngineRuntimeException.h"
#include "NotImplementedException.h"

using TracedExceptionWrapper = boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

#define THROW_EXCEPTION(exception, message) throw boost::enable_error_info( \
  exception(message, __FILE__, __LINE__, __FUNCTION__)) << TracedExceptionWrapper(boost::stacktrace::stacktrace());
