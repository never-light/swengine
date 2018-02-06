#include "Utils.h"

#include <Windows.h>

void copyStringToClipboard(const std::string &s) {
	OpenClipboard(0);
	EmptyClipboard();

	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());

	if (!hg) {
		CloseClipboard();
		return;
	}

	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	GlobalFree(hg);
}
