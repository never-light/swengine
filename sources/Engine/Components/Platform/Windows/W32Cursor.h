#pragma once

#include "../Base/BaseCursor.h"

#include <Engine/Components/ResourceManager/RawImage.h>
#include <Windows.h>

namespace sw::platform::windows
{
/**
 * \brief Mouse cursor
 */
class Cursor : public base::Cursor {
public:
	Cursor();
	Cursor(const RawImage& rawImage);
		
	virtual ~Cursor();

	Cursor(const Cursor& cursor) = delete;
	Cursor& operator=(const Cursor& cursor) = delete;

	/**
	 * \brief Returns native Windows cursor handle
	 * 
	 * \return Native Windows cursor handle
	 */
	HCURSOR getNativeHandle() const;

private:
	/**
	 * \brief Native Windows cursor handle
	 */
	HCURSOR m_handle;
};
}