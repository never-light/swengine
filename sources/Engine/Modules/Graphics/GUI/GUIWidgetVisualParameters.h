#pragma once

#include <memory>

#include <utility>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <optional>

#include "Modules/Graphics/OpenGL/GLTexture.h"

class GUIWidgetVisualParameters {
 public:
  using ParamVec4 = std::optional<glm::vec4>;
  using ParamTexture = std::shared_ptr<GLTexture>;
  using ParamInt = std::optional<int>;

 public:
  GUIWidgetVisualParameters() = default;

  inline void setBackgroundColor(const glm::vec4& color)
  {
    m_backgroundColor = color;
  }

  [[nodiscard]] inline const ParamVec4& getBackgroundColor() const
  {
    return m_backgroundColor;
  }

  void setBackgroundImage(std::shared_ptr<GLTexture> image)
  {
    m_backgroundImage = std::move(image);
  }

  [[nodiscard]] inline std::shared_ptr<GLTexture> getBackgroundImage() const
  {
    return m_backgroundImage;
  }

  inline void setBorderWidth(int width)
  {
    m_borderWidth = width;
  }

  [[nodiscard]] inline const ParamInt& getBorderWidth() const
  {
    return m_borderWidth;
  }

  inline void setBorderColor(const glm::vec4& color)
  {
    m_borderColor = color;
  }

  [[nodiscard]] inline const ParamVec4& getBorderColor() const
  {
    return m_borderColor;
  }

 private:
  ParamVec4 m_backgroundColor{};
  std::shared_ptr<GLTexture> m_backgroundImage;

  ParamInt m_borderWidth{};
  ParamVec4 m_borderColor{};
};
