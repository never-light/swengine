#ifndef GAME_AS_STATIC_LIB
#define SDL_MAIN_HANDLED
#endif

#include <iostream>
#include <Engine/Exceptions/exceptions.h>
#include "Core/GameApplication.h"

#ifdef GAME_AS_STATIC_LIB
int main(int argc, char* argv[])
#else
int gameMain(int argc, char* argv[])
#endif
{
  try {
    GameApplication app(argc, argv);
    return app.execute();
  }
  catch (const EngineRuntimeException& error) {
    spdlog::critical("Engine failed with exception: {}, file: {}, line: {}, function: {}", error.what(),
      error.getFile(), error.getLine(), error.getFunction());

    const boost::stacktrace::stacktrace* trace = boost::get_error_info<TracedExceptionWrapper>(error);

    if (trace) {
      std::string traceRepresentation =
        boost::stacktrace::detail::to_string(&trace->as_vector()[0], trace->size());
      spdlog::debug(traceRepresentation);
    }
  }

  return 0;
}
