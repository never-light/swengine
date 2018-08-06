#include "OpenGL3GraphicsContext.h"

#include <Engine\Exceptions\EngineException.h>
#include "OpenGL3Errors.h"

OpenGL3GraphicsContext::OpenGL3GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, Logger* logger)
	: GraphicsContext(window, viewportWidth, viewportHeight, logger)
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw EngineException("Failed to initialize OpenGL", __FILE__, __LINE__, __FUNCTION__);

	glViewport(0, 0, m_viewportWidth, m_viewportHeight);

#ifdef _DEBUG
	if (GLEW_ARB_debug_output) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB((GLDEBUGPROCARB)&debugOutputCallback, this);
	}
#endif
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

void OpenGL3GraphicsContext::enableWireframeRendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OpenGL3GraphicsContext::disableWireframeRendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

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
	OPENGL3_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGL3GraphicsContext::swapBuffers() {
	glfwSwapBuffers(m_window->getWindowPointer());
}

void OpenGL3GraphicsContext::debugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, GLvoid * userParam)
{
	std::string debugMessage = "[OpenGL] ";

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		debugMessage += "error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debugMessage += "deprecated behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debugMessage += "undefined behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		debugMessage += "portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		debugMessage += "perfomance";
		break;
	case GL_DEBUG_TYPE_OTHER:
		debugMessage += "common";
		break;
	}

	debugMessage += " (" + std::to_string(id) + ", ";

	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		debugMessage += "low";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		debugMessage += "medium";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		debugMessage += "high";
		break;
	}

	debugMessage += ") " + std::string(message);

	OpenGL3GraphicsContext* self = static_cast<OpenGL3GraphicsContext*>(userParam);
	self->m_logger->log(debugMessage);
}