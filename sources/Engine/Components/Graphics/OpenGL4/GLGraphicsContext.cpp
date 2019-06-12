#include "GLGraphicsContext.h"

#include <Engine/Exceptions/EngineException.h>
#include "GLDebug.h"

#include "GLTexture.h"
#include "GLGpuProgram.h"
#include "GLRenderTarget.h"

#include <Engine/config.h>

const float NDC_QUAD_VERTICES_RAW_DATA[] = {
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f
};

GLGraphicsContext::GLGraphicsContext(std::shared_ptr<sw::platform::base::Window> window, RenderTarget* windowRenderTarget, Logger* logger)
	: GraphicsContext(window, windowRenderTarget, logger)
{
	if (gl3wInit())
		throw EngineException("Failed to initialize OpenGL", __FILE__, __LINE__, __FUNCTION__);

	glViewport(0, 0, getViewportWidth(), getViewportHeight());

	if constexpr (sw::DEBUG_LEVEL == sw::DebugLevel::FullDebugging) {
		if (GL_ARB_debug_output) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glDebugMessageCallback(reinterpret_cast<GLDEBUGPROCARB>(&debugOutputCallback), this);
		}
	}

	m_ndcQuadInstance = new GLGeometryInstance();
	m_ndcQuadInstance->setVerticesData(4, sizeof(NDC_QUAD_VERTICES_RAW_DATA), 
		reinterpret_cast<const std::byte*>(NDC_QUAD_VERTICES_RAW_DATA), GeometryInstance::DataUsage::StaticDraw);

	m_ndcQuadInstance->setAttributeDesc(0, 
		GeometryAttributeDesc(GeometryAttributeType::Float, 0, 4, 4 * sizeof(float)));

	m_ndcQuadInstance->create();
}

GLGraphicsContext::~GLGraphicsContext()
{
}

void GLGraphicsContext::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void GLGraphicsContext::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void GLGraphicsContext::setDepthTestFunction(DepthFunction function)
{
	GLenum func;

	if (function == DepthFunction::Less)
		func = GL_LESS;
	else if (function == DepthFunction::LessEqual)
		func = GL_LEQUAL;

	glDepthFunc(func);
}

void GLGraphicsContext::enableWritingToDepthBuffer()
{
	glDepthMask(GL_TRUE);
}

void GLGraphicsContext::disableWritingToDepthBuffer()
{
	glDepthMask(GL_FALSE);
}

void GLGraphicsContext::enableFaceCulling()
{
	glEnable(GL_CULL_FACE);
}

void GLGraphicsContext::disableFaceCulling()
{
	glDisable(GL_CULL_FACE);
}

void GLGraphicsContext::enableBlending()
{
	glEnable(GL_BLEND);
}

void GLGraphicsContext::disableBlending()
{
	glDisable(GL_BLEND);
}

void GLGraphicsContext::setBlendingMode(BlendingMode sourceAffect, BlendingMode destinationAffect)
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

void GLGraphicsContext::setBlendingEquation(BlendingEquation equation)
{
	GLenum glEquation;

	if (equation == BlendingEquation::Add)
		glEquation = GL_FUNC_ADD;
	else if (equation == BlendingEquation::Subtract)
		glEquation = GL_FUNC_SUBTRACT;
	else if (equation == BlendingEquation::ReverseSubtract)
		glEquation = GL_FUNC_REVERSE_SUBTRACT;
	else if (equation == BlendingEquation::Min)
		glEquation = GL_MIN;
	else if (equation == BlendingEquation::Max)
		glEquation = GL_MAX;

	glBlendEquation(glEquation);
}

void GLGraphicsContext::enableScissorTest()
{
	glEnable(GL_SCISSOR_TEST);
}

void GLGraphicsContext::disableScissorTest()
{
	glDisable(GL_SCISSOR_TEST);
}

void GLGraphicsContext::enableWireframeRendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLGraphicsContext::disableWireframeRendering()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLGraphicsContext::setScissorRectangle(const Rect & rectangle)
{
	glScissor(rectangle.getX(), 
		m_window->getClientHeight() - rectangle.getY() - rectangle.getHeight(),
		rectangle.getWidth(), rectangle.getHeight());
}

void GLGraphicsContext::drawFullScreenQuad()
{
	m_ndcQuadInstance->bind();
	m_ndcQuadInstance->draw(GeometryInstance::DrawMode::TrianglesStrip);
}

void GLGraphicsContext::setFaceCullingMode(FaceCullingMode mode)
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

GeometryInstance * GLGraphicsContext::getNDCQuadInstance() const
{
	return m_ndcQuadInstance;
}

GeometryInstance * GLGraphicsContext::createGeometryInstance()
{
	return new GLGeometryInstance();
}

Texture * GLGraphicsContext::createTexture()
{
	return new GLTexture();
}

GpuProgram * GLGraphicsContext::createGpuProgram()
{
	return new GLGpuProgram();
}

RenderTarget * GLGraphicsContext::createRenderTarget()
{
	return new GLRenderTarget();
}

void GLGraphicsContext::debugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, GLvoid * userParam)
{
	std::string debugMessage = "[OpenGL] ";

	if (type == GL_DEBUG_TYPE_OTHER)
		return;

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

	GLGraphicsContext* self = static_cast<GLGraphicsContext*>(userParam);
	self->m_logger->log(debugMessage);
}