#pragma once

#include <catch2/catch.hpp>

#include <Engine/Modules/Graphics/GraphicsSystem/Animation/Skeleton.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachine.h>

#include "utils/utils.h"

Skeleton generateTestSkeleton();
AnimationClip generateTestAnimationClip();
AnimationClipInstance generateTestAnimationClipInstance();
