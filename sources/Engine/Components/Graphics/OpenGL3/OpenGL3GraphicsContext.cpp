#include "OpenGL3GraphicsContext.h"

#include <exception>

OpenGL3GraphicsContext::OpenGL3GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight)
	: GraphicsContext(window, viewportWidth, viewportHeight)
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::exception();

	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
}

OpenGL3GraphicsContext::~OpenGL3GraphicsContext()
{

}

void OpenGL3GraphicsContext::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void OpenGL3GraphicsContext::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void OpenGL3GraphicsContext::setDepthTestFunction(DepthFunction function)
{
	GLenum func;

	if (function == DepthFunction::Less)
		func = GL_LESS;
	else if (function == DepthFunction::LessEqual)
		func = GL_LEQUAL;

	glDepthFunc(func);
}

void OpenGL3GraphicsContext::enableFaceCulling()
{
	glEnable(GL_CULL_FACE);
}

void OpenGL3GraphicsContext::disableFaceCulling()
{
	glDisable(GL_CULL_FACE);
}

void OpenGL3GraphicsContext::enableBlending()
{
	glEnable(GL_BLEND);
}

void OpenGL3GraphicsContext::disableBlending()
{
	glDisable(GL_BLEND);
}

void OpenGL3GraphicsContext::setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect)
{
	GLenum sourceFactor;
	GLenum destinationFactor;

	if (sourceAffect == BlendingMode::Zero)
		sourceFactor = GL_ZERO;
	else if (sourceAffect == BlendingMode::One)
		sourceFactor = GL_ONE;
	else if (sourceAffect == BlendingMode::SrcAlpha)
		sourceFactor = GL_SRC_ALPHA;
	else if (sourceAffect == BlendingMode::OneMinusSrcAlpha)
		sourceFactor = GL_ONE_MINUS_SRC_ALPHA;

	if (destinationAffect == BlendingMode::Zero)
		destinationFactor = GL_ZERO;
	else if (destinationAffect == BlendingMode::One)
		destinationFactor = GL_ONE;
	else if (destinationAffect == BlendingMode::SrcAlpha)
		destinationFactor = GL_SRC_ALPHA;
	else if (destinationAffect == BlendingMode::OneMinusSrcAlpha)
		destinationFactor = GL_ONE_MINUS_SRC_ALPHA;


	glBlendFunc(sourceFactor, destinationFactor);
}

void OpenGL3GraphicsContext::enableScissorTest()
{
	glEnable(GL_SCISSOR_TEST);
}

void OpenGL3GraphicsContext::disableScissorTest()
{
	glDisable(GL_SCISSOR_TEST);
}

#include <iostream>

void OpenGL3GraphicsContext::setScissorRectangle(const Rect & rectangle)
{
	glScissor(rectangle.getX(), 
		m_window->getHeight() - rectangle.getY() - rectangle.getHeight(),
		rectangle.getWidth(), rectangle.getHeight());
}

void OpenGL3GraphicsContext::setFaceCullingMode(FaceCullingMode mode)
{
	GLenum glMode;

	if (mode == FaceCullingMode::Front)
		glMode = GL_FRONT;
	else if (mode == FaceCullingMode::Back)
		glMode = GL_BACK;
	else if (mode == FaceCullingMode::FrontBack)
		glMode = GL_FRONT_AND_BACK;

	glCullFace(glMode);
}

void OpenGL3GraphicsContext::clear(const vector3& color) {
	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL3GraphicsContext::swapBuffers() {
	glfwSwapBuffers(m_window->getWindowPointer());
}