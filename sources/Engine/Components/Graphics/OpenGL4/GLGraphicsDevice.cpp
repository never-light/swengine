#include "GLGraphicsDevice.h"

#include <gl/gl.h>
#include <GL/gl3w.h>
#include <KHR/wglext.h>

#include <Engine/config.h>
#include <Engine/Components/Platform/Windows/W32Window.h>
#include <Engine/Exceptions/EngineException.h>

#include "GLRenderTarget.h"
#include "GLGpuProgram.h"
#include "GLTexture.h"
#include "GLGraphicsContext.h"
#include "GLWindowRenderTarget.h"

namespace WindowsPlatform = sw::platform::windows;

static const int SW_GL_MAJOR = 4;
static const int SW_GL_MINOR = 5;

GLGraphicsDevice::GLGraphicsDevice(std::shared_ptr<sw::platform::base::Window> window, Logger* logger)
	: GraphicDevice()
{
	if constexpr (sw::TARGET_PLATFORM == sw::Platform::Windows) {
		WindowsPlatform::Window* winWindow = dynamic_cast<WindowsPlatform::Window*>(window.get());

		if (winWindow == nullptr) {
			THROW_ENGINE_ERROR("Failed to cast the window to the target platform class");
		}

		HWND dummyWindow = CreateWindow(static_cast<LPCSTR>(winWindow->getNativeClassName()), "DummyWindow", 
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(nullptr), NULL);

		HDC dummyDeviceContext = GetDC(dummyWindow);

		PIXELFORMATDESCRIPTOR dummyPixelFormat;
		ZeroMemory(&dummyPixelFormat, sizeof(dummyPixelFormat));
		dummyPixelFormat.nSize = sizeof(dummyPixelFormat);
		dummyPixelFormat.nVersion = 1;
		dummyPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		dummyPixelFormat.iPixelType = PFD_TYPE_RGBA;
		dummyPixelFormat.cColorBits = 32;
		dummyPixelFormat.cAlphaBits = 8;
		dummyPixelFormat.cDepthBits = 24;

		int dummyPixelFormatId = ChoosePixelFormat(dummyDeviceContext, &dummyPixelFormat);

		if (dummyPixelFormatId == 0) {
			THROW_ENGINE_ERROR("Failed to setup a dummy pixel format");
		}

		if (!static_cast<bool>(SetPixelFormat(dummyDeviceContext, dummyPixelFormatId, &dummyPixelFormat))) {
			THROW_ENGINE_ERROR("Failed to associate the dummy pixel format with device context");
		}

		HGLRC dummyContext = wglCreateContext(dummyDeviceContext);

		if (dummyContext == 0) {
			THROW_ENGINE_ERROR("Failed to create a dummy context");
		}

		if (!static_cast<bool>(wglMakeCurrent(dummyDeviceContext, dummyContext))) {
			THROW_ENGINE_ERROR("Failed to make the dummy context current");
		}

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

		if (wglChoosePixelFormatARB == nullptr) {
			THROW_ENGINE_ERROR("Failed to load choose format WGL extension");
		}

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

		if (wglCreateContextAttribsARB == nullptr) {
			THROW_ENGINE_ERROR("Failed to load create context WGL extension");
		}

		m_deviceContext = GetDC(winWindow->getNativeHandle());

		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		int pixelFormatId;
		UINT availableFormatsNumber;
		bool choosePixelFormatStatus = wglChoosePixelFormatARB(m_deviceContext, pixelAttribs, NULL, 1, &pixelFormatId, &availableFormatsNumber);

		if (choosePixelFormatStatus == false || availableFormatsNumber == 0) {
			THROW_ENGINE_ERROR("Failed to choose pixel format");
		}

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		DescribePixelFormat(m_deviceContext, pixelFormatId, sizeof(pixelFormatDescriptor), &pixelFormatDescriptor);
		SetPixelFormat(m_deviceContext, pixelFormatId, &pixelFormatDescriptor);

		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, SW_GL_MAJOR,
			WGL_CONTEXT_MINOR_VERSION_ARB, SW_GL_MINOR,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, contextAttribs);

		if (m_renderingContext == NULL) {
			THROW_ENGINE_ERROR("Failed to create the rendering context");
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(dummyContext);
		ReleaseDC(dummyWindow, dummyDeviceContext);
		DestroyWindow(dummyWindow);

		if (!wglMakeCurrent(m_deviceContext, m_renderingContext)) {
			THROW_ENGINE_ERROR("Failed to make the rendering context current");
		}

		if (gl3wInit() != 0)
			throw EngineException("Failed to initialize OpenGL", __FILE__, __LINE__, __FUNCTION__);

		RenderTarget* windowRenderTarget = new GLWindowRenderTarget();
		windowRenderTarget->create();

		windowRenderTarget->setSize(window->getClientWidth(), window->getClientHeight());

		m_immediateContext = std::make_shared<GLGraphicsContext>(window, windowRenderTarget, logger);
	}
	else {
		//static_assert(sw::TARGET_PLATFORM != sw::Platform::Windows, "The target platform is wrong");
	}
}

GLGraphicsDevice::~GLGraphicsDevice()
{
	if (m_renderingContext != nullptr) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_renderingContext);
	}
}

std::shared_ptr<GraphicsContext> GLGraphicsDevice::getImmediateContext()
{
	return m_immediateContext;
}

void GLGraphicsDevice::swapBuffers()
{
	SwapBuffers(m_deviceContext);
}

std::shared_ptr<GLGeometryInstance> GLGraphicsDevice::createGeometryInstance()
{
	return std::make_shared<GLGeometryInstance>();
}

std::shared_ptr<Texture> GLGraphicsDevice::createTexture()
{
	return std::make_shared<GLTexture>();
}

std::shared_ptr<GpuProgram> GLGraphicsDevice::createGpuProgram()
{
	return std::make_shared<GLGpuProgram>();
}

std::shared_ptr<RenderTarget> GLGraphicsDevice::createRenderTarget()
{
	return std::make_shared<GLRenderTarget>();
}