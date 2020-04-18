#include "AnimationExporter.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>

AnimationExporter::AnimationExporter()
{

}

void AnimationExporter::exportToFile(const std::string& path,
  const RawSkeletalAnimationClip& animation,
  const AnimationExportOptions& options)
{
  ARG_UNUSED(options);

  spdlog::info("Save animation clip to file: {}", path);
  RawSkeletalAnimationClip::writeToFile(path, animation);
}
