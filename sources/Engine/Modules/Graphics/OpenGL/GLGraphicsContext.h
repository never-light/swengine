#pragma once

#include "GL.h"
#include "Modules/Math/Rect.h"

class GLGraphicsContext {
public:
    GLGraphicsContext(SDL_Window* window);
	~GLGraphicsContext();

    void enableDepthTest();
    void disableDepthTest();

    void setDepthTestFunction(GLenum function);

    void enableWritingToDepthBuffer();
    void disableWritingToDepthBuffer();

    void enableFaceCulling();
    void disableFaceCulling();

    void setFaceCullingMode(GLenum mode);

    void enableBlending();
    void disableBlending();

    void setBlendingMode(GLenum sourceAffect, GLenum destinationAffect);
    void setBlendingEquation(GLenum equation);

    void enableScissorTest();
    void disableScissorTest();

    void enableWireframeRendering();
    void disableWireframeRendering();

    void setScissorRectangle(const Rect& rectangle);

    void swapBuffers();

    int getBufferWidth() const;
    int getBufferHeight() const;
private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;

private:
	static void APIENTRY debugOutputCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		GLvoid* userParam);
};