#pragma once

#include <string>

class Texture {
public:
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};