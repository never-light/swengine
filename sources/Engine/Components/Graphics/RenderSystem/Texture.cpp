#include "Texture.h"

Texture::DataTarget operator+(Texture::DataTarget target, int offset) {
	return (Texture::DataTarget)((unsigned int)target + offset);
}