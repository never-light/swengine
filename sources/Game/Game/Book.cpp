#include "Book.h"

Book::Book(SolidMesh * mesh, BaseMaterial * baseMaterial, Texture* icon,
	const std::string& title, const std::string & text)
	: SolidGameObject(mesh, baseMaterial),
	InventoryObject(icon),
	m_text(text)
{
	setInventoryTitle(title);
	setInventoryUsage(InventoryUsage::Book);
}

Book::~Book()
{
}

const std::string & Book::getText() const
{
	return m_text;
}

void Book::setText(const std::string & text)
{
	m_text = text;
}