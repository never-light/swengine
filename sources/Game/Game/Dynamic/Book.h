#pragma once

#include <Game\SolidGameObject.h>
#include <Game\Game\Inventory\InventoryObject.h>

class Book : public SolidGameObject, public InventoryObject {
public:
	Book(SolidMesh* mesh, BaseMaterial* baseMaterial, Texture* icon, const std::string& title, const std::string& text);
	~Book();

	const std::string& getText() const;
	void setText(const std::string& text);

private:
	std::string m_text;
};