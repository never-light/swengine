#pragma once

#include <QOpenGLWidget>
#include <QDebug>

class ViewportWidget : public QOpenGLWidget
{
public:
	ViewportWidget(QWidget *parent);

protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override
	{

	}

	void paintGL() override;

};
