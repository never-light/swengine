#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLMaterial.h"

class Material {
 public:
  Material(std::unique_ptr<GLMaterial> gpuMaterial);

  [[nodiscard]] const GLMaterial& getGpuMaterial() const;
  [[nodiscard]] GLMaterial& getGpuMaterial();

 private:
  std::unique_ptr<GLMaterial> m_gpuMaterial;
};

