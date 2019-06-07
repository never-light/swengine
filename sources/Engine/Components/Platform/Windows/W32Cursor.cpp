#include "W32Cursor.h"
#include <Engine/Exceptions/EngineException.h>

namespace sw::platform::windows 
{
Cursor::Cursor()
{
	m_handle = LoadCursor(nullptr, IDC_ARROW);
}

Cursor::Cursor(const RawImage& rawImage)
{
	BITMAPV5HEADER bi;
	ICONINFO ii;
	unsigned char* source = reinterpret_cast<unsigned char*>(rawImage.getRawData());

	ZeroMemory(&bi, sizeof(bi));
	bi.bV5Size = sizeof(bi);
	bi.bV5Width = rawImage.getWidth();
	bi.bV5Height = -rawImage.getHeight();
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00ff0000;
	bi.bV5GreenMask = 0x0000ff00;
	bi.bV5BlueMask = 0x000000ff;
	bi.bV5AlphaMask = 0xff000000;

	HDC dc = GetDC(nullptr);
	unsigned char* target = nullptr;
	HBITMAP colorMask = CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, 
		reinterpret_cast<void**>(&target), nullptr, static_cast<DWORD>(0));

	ReleaseDC(nullptr, dc);

	if (!colorMask) {
		THROW_ENGINE_ERROR("Cursor loading failed");
	}

	HBITMAP mask = CreateBitmap(rawImage.getWidth(), rawImage.getHeight(), 1, 1, NULL);
	
	if (!mask) {
		DeleteObject(colorMask);

		THROW_ENGINE_ERROR("Cursor loading failed");
	}

	for (int i = 0; i < rawImage.getWidth() * rawImage.getHeight(); i++)
	{
		target[0] = source[2];
		target[1] = source[1];
		target[2] = source[0];
		target[3] = source[3];
		target += 4;
		source += 4;
	}

	ZeroMemory(&ii, sizeof(ii));
	ii.hbmMask = mask;
	ii.hbmColor = colorMask;

	HICON handle = CreateIconIndirect(&ii);

	DeleteObject(colorMask);
	DeleteObject(mask);

	if (!handle) {
		THROW_ENGINE_ERROR("Cursor loading failed");
	}

	m_handle = handle;
}

Cursor::~Cursor()
{
	DestroyCursor(m_handle);
}

HCURSOR Cursor::getNativeHandle() const
{
	return m_handle;
}

}
