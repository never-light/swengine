#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/OpenGL/GLMaterial.h"

class Material : public Resource {
 public:
  explicit Material(std::unique_ptr<GLMaterial> gpuMaterial);
  ~Material() override = default;

  [[nodiscard]] const GLMaterial& getGpuMaterial() const;
  [[nodiscard]] GLMaterial& getGpuMaterial();

 private:
  std::unique_ptr<GLMaterial> m_gpuMaterial;
};

