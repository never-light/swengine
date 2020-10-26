#pragma once

#include <memory>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResource.h>
#include <Engine/Modules/Graphics/Resources/SkeletalAnimationResource.h>
#include <Engine/Modules/Graphics/Resources/MaterialResource.h>
#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Physics/Resources/CollisionDataResource.h>

inline std::shared_ptr<ResourcesManager> generateTestResourcesManager()
{
  auto resourcesManager = std::make_shared<ResourcesManager>();

  resourcesManager->registerResourceType<Skeleton>("skeleton",
    std::make_unique<SkeletonResource>(resourcesManager.get()));
  resourcesManager->registerResourceType<AnimationClip>("animation",
    std::make_unique<SkeletalAnimationResource>(resourcesManager.get()));
  resourcesManager->registerResourceType<CollisionShape>("collision",
    std::make_unique<CollisionDataResource>(resourcesManager.get()));
  resourcesManager->registerResourceType<GLShader>("shader",
    std::make_unique<ShaderResource>(resourcesManager.get()));
  resourcesManager->registerResourceType<Material>("material",
    std::make_unique<MaterialResource>(resourcesManager.get()));

  return resourcesManager;
}