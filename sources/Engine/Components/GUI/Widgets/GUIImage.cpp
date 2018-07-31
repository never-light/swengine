#include "GUIImage.h"

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
	m_image = image;
}

void GUIImage::render(GeometryStore* quad, GpuProgram* program)
{
	m_image->bind(0);

	program->setParameter("transform.localToWorld", getTransformationMatrix());

	program->setParameter("quad.texture", 0);
	program->setParameter("quad.useTexture", true);
	program->setParameter("quad.useFirstChannel", false);

	quad->drawArrays(GeometryStore::DrawType::Triangles, 0, 6);
}

void GUIImage::update(const MousePosition & mousePosition)
{
}
