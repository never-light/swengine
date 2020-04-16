#define SDL_MAIN_HANDLED

#include <iostream>
#include <spdlog/spdlog.h>

#include <Engine/Exceptions/EngineRuntimeException.h>

#include "MeshToolApplication.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    MeshToolApplication app;
    app.execute(argc, argv);
  }
  catch (const cxxopts::OptionException& ex) {
    spdlog::error("Arguments are invalid: {}", ex.what());
  }
  catch (const EngineRuntimeException& ex) {
    spdlog::error("Processing error: {} [file: {}, line: {}, function: {}",
                  ex.what(), ex.getFile(), ex.getLine(), ex.getFunction());
  }
  catch (const std::exception& ex) {
    spdlog::error("Unknown error: {}", ex.what());
  }

  return 0;
}
