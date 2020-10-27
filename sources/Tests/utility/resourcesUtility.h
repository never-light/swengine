#pragma once

#include <memory>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResourceManager.h>
#include <Engine/Modules/Graphics/Resources/SkeletalAnimationResourceManager.h>
#include <Engine/Modules/Graphics/Resources/MaterialResourceManager.h>
#include <Engine/Modules/Graphics/Resources/ShaderResourceManager.h>
#include <Engine/Modules/Physics/Resources/CollisionShapeResourceManager.h>

inline std::shared_ptr<ResourcesManager> generateTestResourcesManager()
{
  auto resourcesManager = std::make_shared<ResourcesManager>();

  resourcesManager->registerResourceType<Skeleton>("skeleton",
    std::make_unique<SkeletonResourceManager>(resourcesManager.get()));
  resourcesManager->registerResourceType<AnimationClip>("animation",
    std::make_unique<SkeletalAnimationResourceManager>(resourcesManager.get()));
  resourcesManager->registerResourceType<CollisionShape>("collision",
    std::make_unique<CollisionShapeResourceManager>(resourcesManager.get()));
  resourcesManager->registerResourceType<GLShader>("shader",
    std::make_unique<ShaderResourceManager>(resourcesManager.get()));
  resourcesManager->registerResourceType<Material>("material",
    std::make_unique<MaterialResourceManager>(resourcesManager.get()));

  return resourcesManager;
}