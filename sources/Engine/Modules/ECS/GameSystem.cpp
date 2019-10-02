#include "GameSystem.h"

GameSystem::GameSystem()
{
}

GameSystem::~GameSystem()
{
}

void GameSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void GameSystem::render(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void GameSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void GameSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}
