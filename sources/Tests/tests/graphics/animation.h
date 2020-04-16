#pragma once

#include <catch2/catch.hpp>

#include <Engine/Modules/Graphics/GraphicsSystem/Animation/Skeleton.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationClip.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationStatesManager.h>

#include "utils/utils.h"

Skeleton generateTestSkeleton();
SkeletalAnimationClip generateTestAnimationClip();
SkeletalAnimationClipInstance generateTestAnimationClipInstance();
