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