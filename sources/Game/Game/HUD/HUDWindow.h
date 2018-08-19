#pragma once

#include <Engine\Components\GUI\Widgets\GUIWindow.h>
#include <functional>

class HUDWindow : public GUIWindow {
private:
	using ChangeStateInternalCallback = std::function<void(HUDWindow*)>;

public:
	HUDWindow();
	virtual ~HUDWindow();

	virtual void close();
	virtual void open();

	bool isOpened() const;

	void setChageStateInternalCallbacks(const ChangeStateInternalCallback& openCallback, const ChangeStateInternalCallback& closeCallback);

private:
	ChangeStateInternalCallback m_openInternalCallback;
	ChangeStateInternalCallback m_closeInternalCallback;
};