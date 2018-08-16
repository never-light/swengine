#pragma once

#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include "Light.h"

#include <Game\Graphics\Materials\BaseMaterial.h>
#include <Game\Graphics\Renderable.h> 

#include <Game\Graphics\Primitives\NDCQuadPrimitive.h>
#include <Game\Graphics\Primitives\SpherePrimitive.h>

class LevelRenderer {
public:
	LevelRenderer(GraphicsContext* graphicsContext, 
		GraphicsResourceFactory* graphicsResourceFactory,
		GpuProgram* deferredLightingProgram);
	~LevelRenderer();

	void registerLightSource(Light* lightSource);
	void updateLightSource(const Light* lightSource);

	void removeLightSource(const Light* lightSource);

	void setActiveCamera(const Camera* camera);
	void render();

	void registerBaseMaterial(BaseMaterial* baseMaterial);
	
	void addRenderableObject(Renderable* object);
	void removeRenderableObject(Renderable* object);

	void enableGammaCorrection();
	void disableGammaCorrection();
	bool isGammaCorrectionEnabled();

	void setGamma(float gamma);
	float getGamma() const;

protected:
	void prepareBaseMaterials();
	void showGBuffer();

	float calculateLightSourceSphereRadius(const Light* light) const;
	void passLightSourceDataToGpuProgram(size_t index, const Light* light, GpuProgram* gpuProgram) const;

	void initializeRenderTarget();
	Texture* createGBufferColorTexture();
	Texture* createGBufferDepthStencilTexture();

protected:
	float m_gamma;
	bool m_isGammaCorrectionEnabled;

protected:
	const Camera* m_activeCamera;

	GraphicsContext * m_graphicsContext;
	GraphicsResourceFactory* m_graphicsResourceFactory;

	std::vector<const Light*> m_lightsSources;

	std::vector<BaseMaterial*> m_baseMaterials;
	std::vector<Renderable*> m_renderableObjects;

protected:
	GpuProgram* m_deferredLightingProgram;

	NDCQuadPrimitive * m_ndcQuad;
	SpherePrimitive* m_sphere;

	RenderTarget * m_gBufferTarget;

	Texture* m_gBufferAlbedo;
	Texture* m_gBufferNormals;
	Texture* m_gBufferPosition;
	Texture* m_gBufferUV;
	Texture* m_gBufferDepthStencil;

	static const size_t ALBEDO_BUFFER_INDEX = 0;
	static const size_t NORMALS_BUFFER_INDEX = 1;
	static const size_t POSITION_BUFFER_INDEX = 2;
	static const size_t UV_BUFFER_INDEX = 3;
};