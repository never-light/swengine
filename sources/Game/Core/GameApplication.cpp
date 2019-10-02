#include "GameApplication.h"

GameApplication::GameApplication(int argc, char* argv[])
    : BaseGameApplication(argc, argv, "Game", 640, 480)
{

}

GameApplication::~GameApplication()
{

}

void GameApplication::render()
{
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
