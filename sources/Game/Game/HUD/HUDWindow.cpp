#include "HUDWindow.h"

HUDWindow::HUDWindow()
	: GUIWindow()
{
}

HUDWindow::~HUDWindow()
{
}

void HUDWindow::close()
{
	GUIWindow::hide();

	m_closeInternalCallback(this);
}

void HUDWindow::open()
{
	GUIWindow::show();

	m_openInternalCallback(this);
}

bool HUDWindow::isOpened() const
{
	return GUIWindow::isVisible();
}

void HUDWindow::setChageStateInternalCallbacks(const ChangeStateInternalCallback & openCallback, const ChangeStateInternalCallback & closeCallback)
{
	m_openInternalCallback = openCallback;
	m_closeInternalCallback = closeCallback;
}
