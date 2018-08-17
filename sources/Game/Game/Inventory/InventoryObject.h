#pragma once

#include <functional>

#include <Engine\Components\Graphics\RenderSystem\Texture.h>

class InventoryObject {
public:
	enum class InventoryUsage {
		Simple, Book
	};

public:
	using ActionCallback = std::function<void(InventoryObject*)>;

public:
	InventoryObject(Texture* icon);
	virtual ~InventoryObject();

	Texture* getInventoryIcon() const;

	void triggerUse();
	void setUseCallback(const ActionCallback& callback);

	const std::string& getInventoryTitle() const;
	void setInventoryTitle(const std::string& title);

	void setInventoryUsage(InventoryUsage usage);
	InventoryUsage getInventoryUsage() const;
protected:
	Texture* m_inventoryIcon;
	std::string m_inventoryTitle;

	ActionCallback m_useCallback;

	InventoryUsage m_inventoryUsage;
};