#pragma once

#include <memory>
#include <string>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeletalAnimationClip.h>

struct AnimationExportOptions {

};

class AnimationExporter {
 public:
  AnimationExporter();

  void exportToFile(const std::string& path,
    const RawSkeletalAnimationClip& animation,
    const AnimationExportOptions& options);
};
