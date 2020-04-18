#include "precompiled.h"

#pragma hdrstop

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "SkeletonResource.h"
#include "Exceptions/exceptions.h"

#include "Modules/Graphics/Resources/Raw/RawSkeleton.h"

SkeletonResource::SkeletonResource()
{

}

SkeletonResource::~SkeletonResource()
{
  SW_ASSERT(m_skeleton.use_count() <= 1);
}

void SkeletonResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
  ARG_UNUSED(resourceManager);

  SW_ASSERT(m_skeleton == nullptr);

  SkeletonResourceParameters parameters = declaration.getParameters<SkeletonResourceParameters>();

  if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
    m_skeleton = loadFromFile(sourceFile->path, parameters);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load skeleton resource from invalid source");
  }
}

void SkeletonResource::unload()
{
  SW_ASSERT(m_skeleton.use_count() == 1);

  m_skeleton.reset();
}

bool SkeletonResource::isBusy() const
{
  return m_skeleton.use_count() > 1;
}

std::shared_ptr<Skeleton> SkeletonResource::loadFromFile(const std::string& path,
  const SkeletonResourceParameters& parameters)
{
  ARG_UNUSED(parameters);

  // Read raw mesh
  RawSkeleton rawSkeleton = RawSkeleton::readFromFile(path);

  // Convert raw skeleton to internal skeleton object
  std::vector<Bone> bones(rawSkeleton.bones.size());

  for (size_t boneIndex = 0; boneIndex < rawSkeleton.bones.size(); boneIndex++) {
    const RawBone& rawBone = rawSkeleton.bones[boneIndex];

    bones[boneIndex].setParentId(rawBone.parentId);
    bones[boneIndex].setName(std::string(rawBone.name));
    bones[boneIndex].setInverseBindPoseMatrix(rawMatrix4ToGLMMatrix4(rawBone.inverseBindPoseMatrix));
  }

  std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(bones);

  return skeleton;
}

SkeletonResource::ParametersType SkeletonResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
  const ParametersType& defaultParameters)
{
  ARG_UNUSED(declarationNode);

  ParametersType parameters = defaultParameters;

  return parameters;
}

std::shared_ptr<Skeleton> SkeletonResource::getSkeleton() const
{
  return m_skeleton;
}
