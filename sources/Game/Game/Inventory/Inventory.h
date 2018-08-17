#pragma once

#include <vector>

#include "InventoryObject.h"

class Inventory {
public:
	Inventory();
	~Inventory();

	void addObject(InventoryObject* object);
	const std::vector<InventoryObject*>& getObjects() const;
protected:
	std::vector<InventoryObject*> m_objects;
};