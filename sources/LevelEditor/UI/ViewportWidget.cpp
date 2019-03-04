#include "ViewportWidget.h"

ViewportWidget::ViewportWidget(QWidget * parent)
	: QOpenGLWidget(parent)
{
	QSurfaceFormat format;

	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 5);
	format.setOption(QSurfaceFormat::DebugContext);

	setFormat(format);
}

void ViewportWidget::initializeGL()
{
	// Set up the rendering context, load shaders and other resources, etc.:
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	qDebug() << "version: "
		<< QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	qDebug() << "GSLS version: "
		<< QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void ViewportWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
}