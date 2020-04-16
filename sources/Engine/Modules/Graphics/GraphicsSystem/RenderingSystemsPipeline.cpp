#include "precompiled.h"

#pragma hdrstop

#include "RenderingSystemsPipeline.h"
#include "SharedGraphicsState.h"
#include "DebugPainter.h"

RenderingSystemsPipeline::RenderingSystemsPipeline(std::shared_ptr<GameWorld> gameWorld,
                                                   std::shared_ptr<GLGraphicsContext> graphicsContext,
                                                   std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
    : GameSystemsGroup(gameWorld),
      m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState),
      m_deferredAccumulationMaterial(std::make_shared<Material>(std::make_unique<GLMaterial>())) {
  GLMaterial& gpuMaterial = m_deferredAccumulationMaterial->getGpuMaterial();
  gpuMaterial.setBlendingMode(BlendingMode::Disabled);
  gpuMaterial.setDepthTestMode(DepthTestMode::NotEqual);
  gpuMaterial.setDepthWritingMode(DepthWritingMode::Disabled);
  gpuMaterial.setFaceCullingMode(FaceCullingMode::Disabled);
  gpuMaterial.setPolygonFillingMode(PolygonFillingMode::Fill);
}

void RenderingSystemsPipeline::addGameSystem(std::shared_ptr<GameSystem> system) {
  SW_ASSERT(dynamic_cast<RenderingSystem*>(system.get()) != nullptr);

  GameSystemsGroup::addGameSystem(system);
}

void RenderingSystemsPipeline::render(GameWorld* gameWorld) {
  // TODO: get rid of buffers clearing and copying as possible
  // Use depth swap trick to avoid depth buffer clearing
  m_sharedGraphicsState->getDeferredFramebuffer().clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 0);
  m_sharedGraphicsState->getDeferredFramebuffer().clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 1);
  m_sharedGraphicsState->getDeferredFramebuffer().clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 2);

  m_sharedGraphicsState->getDeferredFramebuffer().clearDepthStencil(1.0f, 0);

  for (auto& system : getGameSystems()) {
    RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
    renderingSystem->renderDeferred(gameWorld);
  }

  GLShadersPipeline* acucmulationPipeline = m_deferredAccumulationMaterial->getGpuMaterial().getShadersPipeline().get();
  GLShader* accumulationFragmentShader = acucmulationPipeline->getShader(GL_FRAGMENT_SHADER);
  const GLFramebuffer& deferredFramebuffer = m_sharedGraphicsState->getDeferredFramebuffer();

  accumulationFragmentShader->setParameter("gBuffer.albedo",
                                           *deferredFramebuffer.getColorComponent(0).get(), 0);

  accumulationFragmentShader->setParameter("gBuffer.normals",
                                           *deferredFramebuffer.getColorComponent(1).get(), 1);

  accumulationFragmentShader->setParameter("gBuffer.positions",
                                           *deferredFramebuffer.getColorComponent(2).get(), 2);

  m_graphicsContext->executeRenderTask(RenderTask{
      &m_deferredAccumulationMaterial->getGpuMaterial(),
      &m_graphicsContext->getNDCTexturedQuad(),
      0, 6,
      GL_TRIANGLES,
      &m_sharedGraphicsState->getForwardFramebuffer()
  });

  for (auto& system : getGameSystems()) {
    RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
    renderingSystem->renderForward(gameWorld);
  }

  for (auto& system : getGameSystems()) {
    RenderingSystem* renderingSystem = dynamic_cast<RenderingSystem*>(system.get());
    renderingSystem->renderPostProcess(gameWorld);
  }

  m_sharedGraphicsState->getForwardFramebuffer().copyColor(m_graphicsContext->getDefaultFramebuffer());
  m_sharedGraphicsState->getForwardFramebuffer().copyDepthStencil(m_graphicsContext->getDefaultFramebuffer());

  DebugPainter::flushRenderQueue(m_graphicsContext.get());
}

void RenderingSystemsPipeline::setDeferredAccumulationShadersPipeline(std::shared_ptr<GLShadersPipeline> pipeline) {
  m_deferredAccumulationMaterial->getGpuMaterial().setShadersPipeline(pipeline);
}

std::shared_ptr<GLShadersPipeline> RenderingSystemsPipeline::getDeferredAccumulationShadersPipeline() const {
  return m_deferredAccumulationMaterial->getGpuMaterial().getShadersPipeline();
}
