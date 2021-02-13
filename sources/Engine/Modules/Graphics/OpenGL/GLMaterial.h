#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <memory>
#include <utility>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "GLTexture.h"

#include "Modules/Graphics/BaseGraphicsBackend/GpuStateParameters.h"
#include "Modules/Graphics/BaseGraphicsBackend/ShadingParameters.h"

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/Resources/ShaderResourceManager.h"
#include "Modules/Math/Rect.h"

class GLShadingParametersBaseBinder {
 public:
  explicit GLShadingParametersBaseBinder(std::unique_ptr<ShadingParametersBaseSet> parametersSet);
  virtual ~GLShadingParametersBaseBinder() = default;

  [[nodiscard]] const ShadingParametersBaseSet& getParametersSet() const;
  [[nodiscard]] ShadingParametersBaseSet& getParametersSet();

  virtual void bindParameters(GLShadersPipeline& shadersPipeline) = 0;

 private:
  std::unique_ptr<ShadingParametersBaseSet> m_parametersSet;
};

class GLShadingParametersGenericBinder : public GLShadingParametersBaseBinder {
 public:
  explicit GLShadingParametersGenericBinder(std::unique_ptr<ShadingParametersBaseSet> parametersSet);
  ~GLShadingParametersGenericBinder() override = default;

  void bindParameters(GLShadersPipeline& shadersPipeline) override;
};

class GLShadingParametersGUIBinder : public GLShadingParametersBaseBinder {
 public:
  explicit GLShadingParametersGUIBinder(std::unique_ptr<ShadingParametersBaseSet> parametersSet);
  ~GLShadingParametersGUIBinder() override = default;

  void bindParameters(GLShadersPipeline& shadersPipeline) override;
};

class GLShadingParametersOpaqueMeshBinder : public GLShadingParametersBaseBinder {
 public:
  explicit GLShadingParametersOpaqueMeshBinder(std::unique_ptr<ShadingParametersBaseSet> parametersSet);
  ~GLShadingParametersOpaqueMeshBinder() override = default;

  void bindParameters(GLShadersPipeline& shadersPipeline) override;
};

class GLGraphicsContext;

class GLMaterial : public Resource {
 public:
  GLMaterial(RenderingStage renderingStage,
    std::shared_ptr<GLShadersPipeline> shadersPipeline,
    GpuStateParameters gpuState,
    std::unique_ptr<ShadingParametersBaseSet> shadingParameters);

  ~GLMaterial() override;

  [[nodiscard]] const GLShadersPipeline& getShadersPipeline() const;
  [[nodiscard]] GLShadersPipeline& getShadersPipeline();

  [[nodiscard]] RenderingStage getRenderingStage() const;
  [[nodiscard]] const GpuStateParameters& getGpuStateParameters() const;

  [[nodiscard]] GLShadingParametersBaseBinder* getGLParametersBinder() const;

  [[nodiscard]] const ShadingParametersBaseSet& getParametersSet() const;
  [[nodiscard]] ShadingParametersBaseSet& getParametersSet();

 private:
  static std::unique_ptr<GLShadingParametersBaseBinder> createShadingParametersBinder(
    std::unique_ptr<ShadingParametersBaseSet> shadingParameters);

 private:
  std::shared_ptr<GLShadersPipeline> m_shadersPipeline;

  RenderingStage m_renderingStage = RenderingStage::Count;
  GpuStateParameters m_gpuStateParameters;
  std::unique_ptr<GLShadingParametersBaseBinder> m_shadingParametersBinder;

 private:
  friend class GLGraphicsContext;
};
