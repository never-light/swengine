#pragma once

#include <Engine\Components\GUI\GUIWidget.h>
#include <Engine\Components\GUI\Widgets\GUIText.h>
#include <Engine\Components\GUI\Widgets\GUIBlock.h>
#include <Engine\Components\GUI\Widgets\GUIImage.h>

#include <Game\Game\HUD\HUDWindow.h>

#include "Inventory.h"

class InventoryViewer : public HUDWindow {
public:
	InventoryViewer(Inventory* inventory, GraphicsResourceFactory* graphicsResourceFactory, Font* textFont);
	~InventoryViewer();

	virtual void render(GeometryStore* quad, GpuProgram* program) override;
	virtual void onHover(const MousePosition& mousePosition) override;
	
	virtual void onClick(const MousePosition& mousePosition, MouseButton button) override;

	virtual void close() override;

protected:
	InventoryObject * findHoveredObject(const MousePosition& mousePosition) const;

	void openSubWindow(const std::string& title);
	void closeSubWindow();
protected:
	Inventory* m_inventory;

	GUIBlock* m_itemBlock;
	GUIImage* m_itemImage;

	GUIText* m_hoverText;
	bool m_hoverTextVisible;

	uivector2 m_itemBlockMargin;

	GUIWindow* m_subWindow;
	GUIText* m_subWindowText;
	GUIText* m_subWindowTitle;

	bool m_subWindowOpened;
private:
	GraphicsResourceFactory * m_graphicsResourceFactory;
	Font* m_textFont;
};