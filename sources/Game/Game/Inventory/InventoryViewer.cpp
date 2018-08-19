#include "InventoryViewer.h"

#include <Game\GameObject.h>
#include <Game\Game\Dynamic\Book.h>

InventoryViewer::InventoryViewer(Inventory* inventory, GraphicsResourceFactory* graphicsResourceFactory, Font* textFont)
	: m_inventory(inventory), 
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_textFont(textFont),
	m_itemBlockMargin(10, 10),
	m_hoverTextVisible(false),
	m_subWindowOpened(false)
{
	m_hoverText = new GUIText(m_graphicsResourceFactory);
	m_hoverText->setFont(m_textFont);
	m_hoverText->setFontSize(10);
	m_hoverText->setColor(1.0, 1.0, 1.0);

	m_itemBlock = new GUIBlock();
	m_itemBlock->setColor(vector4(0.168f, 0.172f, 0.25f, 0.8f));
	m_itemBlock->setSize(128, 128);

	m_itemImage = new GUIImage(nullptr);
	m_itemImage->setSize(100, 100);

	m_subWindow = new GUIWindow();
	m_subWindow->setSize(600, 400);
	m_subWindow->setBackgroundColor(vector4(0.8945f, 0.8203f, 0.6563f, 1.0f));
	m_subWindow->enableBackgroundRendering();
	m_subWindow->hide();

	this->addWidget(m_subWindow);

	m_subWindowTitle = new GUIText(m_graphicsResourceFactory);
	m_subWindowTitle->setFont(m_textFont);
	m_subWindowTitle->setFontSize(12);
	m_subWindowTitle->setColor(0.0f, 0.0f, 0.0f);

	m_subWindow->addWidget(m_subWindowTitle);

	m_subWindowText = new GUIText(m_graphicsResourceFactory);
	m_subWindowText->setFont(m_textFont);
	m_subWindowText->setFontSize(10);
	m_subWindowText->setColor(0.0f, 0.0f, 0.0f);

	m_subWindow->addWidget(m_subWindowText);
}

InventoryViewer::~InventoryViewer()
{
	delete m_subWindowTitle;
	delete m_subWindowText;
	delete m_subWindow;

	delete m_itemImage;
	delete m_itemBlock;
	delete m_hoverText;
}

void InventoryViewer::render(GeometryStore * quad, GpuProgram * program)
{
	GUIWindow::renderBackground(quad, program);

	uivector2 newPosition = m_position + m_itemBlockMargin;

	for (const InventoryObject* object : m_inventory->getObjects()) {
		m_itemBlock->setPosition(newPosition);
		m_itemBlock->render(quad, program);

		m_itemImage->setImage(object->getInventoryIcon());
		m_itemImage->setPosition(newPosition + uivector2(14, 14));

		m_itemImage->render(quad, program);

		newPosition.x += m_itemBlock->getWidth() + m_itemBlockMargin.x;

		if (newPosition.x >= this->getWidth()) {
			newPosition.x = m_position.x + m_itemBlockMargin.x;
			newPosition.y += m_itemBlock->getHeight() + m_itemBlockMargin.y;
		}
	}

	if (m_hoverTextVisible)
		m_hoverText->render(quad, program);

	GUIWindow::renderWidgets(quad, program);
}

void InventoryViewer::onHover(const MousePosition & mousePosition)
{
	if (m_subWindowOpened) {
		m_hoverTextVisible = false;
		return;
	}

	InventoryObject* hoveredObject = findHoveredObject(mousePosition);

	if (hoveredObject != nullptr) {
		m_hoverTextVisible = true;
		m_hoverText->setPosition(static_cast<uint32>(mousePosition.x) + 15, static_cast<uint32>(mousePosition.y) + 8);
		m_hoverText->setText(hoveredObject->getInventoryTitle());
	}
	else {
		m_hoverTextVisible = false;
	}
}

void InventoryViewer::onClick(const MousePosition & mousePosition, MouseButton button)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (m_subWindowOpened) {
			if (!isMouseInWidgetArea(mousePosition, m_subWindow->getPosition(), m_subWindow->getSize())) {
				closeSubWindow();
			}
		}
		else {

			InventoryObject* hoveredObject = findHoveredObject(mousePosition);

			if (hoveredObject == nullptr)
				return;

			dynamic_cast<GameObject*>(hoveredObject)->triggerUse();

			if (hoveredObject->getInventoryUsage() == InventoryObject::InventoryUsage::Book) {

				Book* book = static_cast<Book*>(hoveredObject);

				openSubWindow(book->getInventoryTitle());

				m_subWindowText->setText(book->getText());
			}
		}
	}
}

void InventoryViewer::close()
{
	closeSubWindow();

	HUDWindow::close();
}

InventoryObject * InventoryViewer::findHoveredObject(const MousePosition& mousePosition) const
{
	uivector2 currentPosition = m_position + m_itemBlockMargin;

	for (InventoryObject* object : m_inventory->getObjects()) {
		if (isMouseInWidgetArea(mousePosition, currentPosition, m_itemBlock->getSize())) {
			return object;
		}

		currentPosition.x += m_itemBlock->getWidth() + m_itemBlockMargin.x;

		if (currentPosition.x >= this->getWidth()) {
			currentPosition.x = m_position.x + m_itemBlockMargin.x;
			currentPosition.y += m_itemBlock->getHeight() + m_itemBlockMargin.y;
		}
	}

	return nullptr;
}

void InventoryViewer::openSubWindow(const std::string& title)
{
	m_subWindow->setPosition(m_position.x + m_size.x / 2 - m_subWindow->getWidth() / 2,
		m_position.y + m_size.y / 2 - m_subWindow->getHeight() / 2);

	m_subWindowTitle->setPosition(m_subWindow->getPosition() + uivector2(10, 10));
	m_subWindowTitle->setText(title);

	m_subWindowText->setPosition(m_subWindowTitle->getPosition() + uivector2(0, m_subWindowTitle->getHeight() + 15));

	m_subWindow->show();
	m_subWindowOpened = true;

}

void InventoryViewer::closeSubWindow()
{
	m_subWindow->hide();
	m_subWindowOpened = false;
}
