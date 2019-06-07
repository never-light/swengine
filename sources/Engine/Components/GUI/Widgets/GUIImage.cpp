#include "GUIImage.h"

#include <Engine/assertions.h>

GUIImage::GUIImage(Texture * image)
	: m_image(image)
{
}

GUIImage::~GUIImage()
{
}

Texture* GUIImage::getImage() const
{
	return m_image;
}

void GUIImage::setImage(Texture* image)
{
	_assert(image != nullptr);

	m_image = image;
}

void GUIImage::render(GeometryInstance* quad, GpuProgram* program)
{
	m_image->bind(0);

	program->setParameter("transform.localToWorld", getTransformationMatrix());

	program->setParameter("quad.texture", 0);
	program->setParameter("quad.useTexture", true);
	program->setParameter("quad.useFirstChannel", false);

	quad->draw(GeometryInstance::DrawMode::Triangles, 0, 6);
}

void GUIImage::update(const CursorPosition& mousePosition)
{
}
