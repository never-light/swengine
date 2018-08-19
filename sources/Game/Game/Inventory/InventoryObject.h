#pragma once

#include <functional>

#include <Engine\Components\Graphics\RenderSystem\Texture.h>

class InventoryObject {
public:
	enum class InventoryUsage {
		Simple, Book
	};

public:
	InventoryObject(Texture* icon);
	virtual ~InventoryObject();

	Texture* getInventoryIcon() const;

	const std::string& getInventoryTitle() const;
	void setInventoryTitle(const std::string& title);

	void setInventoryUsage(InventoryUsage usage);
	InventoryUsage getInventoryUsage() const;
protected:
	Texture* m_inventoryIcon;
	std::string m_inventoryTitle;

	InventoryUsage m_inventoryUsage;
};