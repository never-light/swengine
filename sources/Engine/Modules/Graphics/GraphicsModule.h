#pragma once

#include <memory>
#include "OpenGL/GLGraphicsContext.h"

class GraphicsModule final {
 public:
  explicit GraphicsModule(SDL_Window* window);
  ~GraphicsModule();

  [[nodiscard]] std::shared_ptr<GLGraphicsContext> getGraphicsContext() const;

 private:
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
};
