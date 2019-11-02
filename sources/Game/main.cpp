#define SDL_MAIN_HANDLED

#include <iostream>
#include "Core/GameApplication.h"

using namespace std;

int main(int argc, char* argv[])
{
    GameApplication app(argc, argv);
    return app.execute();
}
