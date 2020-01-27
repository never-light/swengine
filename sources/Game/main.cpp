#define SDL_MAIN_HANDLED

#include <iostream>
#include "Core/GameApplication.h"

using namespace std;

int main(int argc, char* argv[])
{
    try {
        GameApplication app(argc, argv);
        return app.execute();
    }
    catch (const EngineRuntimeException& error) {
        spdlog::critical("Engine failed with exception: {}, file: {}, line: {}, function: {}", error.what(),
                         error.getFile(), error.getLine(), error.getFunction());
    }
    catch (const std::exception& error) {
        spdlog::critical("Engine failed with std::exception: {}", error.what());
    }
    catch (...) {
        spdlog::critical("Engine failed with unknown exception");
    }

}
