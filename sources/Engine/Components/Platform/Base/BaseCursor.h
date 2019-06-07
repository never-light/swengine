#pragma once

#include <Engine/Components/ResourceManager/RawImage.h>

namespace sw::platform::base
{
	class Cursor {
	public:
		Cursor() {};
		Cursor(const RawImage& image) {};
		virtual ~Cursor() {};
	};
}