#pragma once

#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Debugging\Log.h>
#include <Engine\Components\GUI\GUI.h>
#include <Engine\Components\Graphics\GraphicsManager.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\SceneManager\SceneManager.h>
#include <Engine\Components\ConfigManager\ConfigManager.h>
#include <Engine\ServiceLocator.h>
#include <Engine\Utils\Utils.h>

void InitializeEngine(Window* window);
void ShutdownEngine();