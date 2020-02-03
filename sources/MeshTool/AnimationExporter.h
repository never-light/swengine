#pragma once

#include <memory>
#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct AnimationExportOptions {

};

class AnimationExporter
{
public:
    AnimationExporter();

    void exportToFile(const std::string& path,
                      const RawSkeletalAnimation& animation,
                      const AnimationExportOptions& options);
};
