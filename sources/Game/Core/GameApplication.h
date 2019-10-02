#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>

class GameApplication : public BaseGameApplication
{
public:
    GameApplication(int argc, char* argv[]);
    ~GameApplication() override;

    void render() override;
};

