#pragma once

#include <Engine/Components/Math/Math.h>
#include <Engine/Components/Debugging/Logger.h>
#include <Engine/Components/GUI/GUI.h>
#include <Engine/Components/Graphics/GraphicsSystem.h>
#include <Engine/Components/ResourceManager/ResourceManager.h>
#include <Engine/Components/InputManager/InputManager.h>
#include <Engine/Components/SceneManager/SceneManager.h>
#include <Engine/Utils/Utils.h>
#include <Engine/assertions.h>

void InitializeEngine(Window* window);
void ShutdownEngine();