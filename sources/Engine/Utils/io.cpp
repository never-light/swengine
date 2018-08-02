#include "io.h"

#include <Windows.h>

void IOUtils::copyToClipboard(const std::string & string)
{
	OpenClipboard(0);
	EmptyClipboard();

	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, string.size());

	if (!hg) {
		CloseClipboard();
		return;
	}

	memcpy(GlobalLock(hg), string.c_str(), string.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	GlobalFree(hg);
}

void IOUtils::showMessageBox(const std::string & caption, const std::string & message, MessageBoxButtons buttons, MessageBoxType type)
{
	UINT displayFlags = 0;

	if (type == MessageBoxType::Default)
		displayFlags |= MB_ICONINFORMATION;
	else if (type == MessageBoxType::Error)
		displayFlags |= MB_ICONERROR;

	if (buttons == MessageBoxButtons::OK)
		displayFlags |= MB_OK;

	MessageBox(NULL, message.c_str(), caption.c_str(), displayFlags);
}
