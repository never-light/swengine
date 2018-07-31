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