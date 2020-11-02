#pragma once


enum class DepthTestMode {
  Disabled, Less, LessOrEqual, NotEqual
};

enum class FaceCullingMode {
  Disabled, Back, Front
};

enum class PolygonFillingMode {
  Fill, Wireframe
};

enum class BlendingMode {
  Disabled, Alpha_OneMinusAlpha
};

enum class DepthWritingMode {
  Disabled, Enabled
};

enum class ScissorsTestMode {
  Disabled, Enabled
};

enum class RenderingStage {
  Deferred,
  Forward,
  ForwardDebug,
  ForwardEnvironment,
  PostProcess,
  GUI,
  Count
};

class GpuStateParameters {
 public:
  GpuStateParameters() = default;

  void setDepthTestMode(DepthTestMode mode);
  [[nodiscard]] DepthTestMode getDepthTestMode() const;

  void setFaceCullingMode(FaceCullingMode mode);
  [[nodiscard]] FaceCullingMode getFaceCullingMode() const;

  void setPolygonFillingMode(PolygonFillingMode mode);
  [[nodiscard]] PolygonFillingMode getPolygonFillingMode() const;

  void setBlendingMode(BlendingMode mode);
  [[nodiscard]] BlendingMode getBlendingMode() const;

  void setDepthWritingMode(DepthWritingMode mode);
  [[nodiscard]] DepthWritingMode getDepthWritingMode() const;

  void setScissorsTestMode(ScissorsTestMode mode);
  [[nodiscard]] ScissorsTestMode getScissorsTestMode() const;

 private:
  DepthTestMode m_depthTestMode = DepthTestMode::LessOrEqual;
  FaceCullingMode m_faceCullingMode = FaceCullingMode::Disabled;
  PolygonFillingMode m_polygonFillingMode = PolygonFillingMode::Fill;
  BlendingMode m_materialBlendingMode = BlendingMode::Disabled;
  DepthWritingMode m_depthWritingMode = DepthWritingMode::Disabled;
  ScissorsTestMode m_scissorsTestMode = ScissorsTestMode::Disabled;
};
