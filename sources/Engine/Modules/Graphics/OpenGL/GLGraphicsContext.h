#pragma once

#include "GL.h"
#include "Modules/Math/Rect.h"

#include "GLGeometryStore.h"
#include "GLShadersPipeline.h"
#include "GLMaterial.h"

class SharedGraphicsState;
class Transform;

struct RenderTask {
    GLMaterial* material;
    GLGeometryStore* geometryStore;

    size_t startOffset;
    size_t partsCount;

    GLenum primitivesType = GL_TRIANGLES;
};

class GLGraphicsContext {
public:
    GLGraphicsContext(SDL_Window* window);
	~GLGraphicsContext();

    void enableWritingToDepthBuffer();
    void disableWritingToDepthBuffer();

    void enableScissorTest();
    void disableScissorTest();

    void setScissorRectangle(const Rect& rectangle);

    void swapBuffers();

    int getBufferWidth() const;
    int getBufferHeight() const;

    float getBufferAspectRatio() const;

    void setDepthTestMode(DepthTestMode mode);
    void setFaceCullingMode(FaceCullingMode mode);
    void setPolygonFillingMode(PolygonFillingMode mode);
    void setBlendingMode(BlendingMode mode);
    void setDepthWritingMode(DepthWritingMode mode);

    void applyMaterial(const GLMaterial& material);
    void executeRenderTask(const RenderTask& task);

private:
    void applyContextChange();
    void resetMaterial();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    GLMaterial* m_currentMaterial = nullptr;

private:
	static void APIENTRY debugOutputCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		GLvoid* userParam);
};
