#include "CodePanel.h"

CodePanel::CodePanel(GraphicsContext* graphicsContext, Font * textFont)
	: m_graphicsContext(graphicsContext),
	m_textFont(textFont),
	m_successulEnterCallback(nullptr),
	m_buttonSize(64, 64),
	m_panelPadding(20, 20),
	m_buttonsOffset(15, 15)
{
	unsigned int panelWidth = m_buttonSize.x * 3 + 2 * m_buttonsOffset.x + m_panelPadding.x * 2;
	unsigned int panelHeight = 36 +
		m_buttonSize.y * 4 + 3 * m_buttonsOffset.y + m_panelPadding.y * 2;

	setSize(panelWidth, panelHeight);

	for (size_t digitIndex = 0; digitIndex < 10; digitIndex++) {
		GUIButton* button = new GUIButton(m_graphicsContext, m_textFont);
		
		button->setBackgroundColor(vector4(0.148f, 0.122f, 0.15f, 1.0f));
		button->setHoverBackgroundColor(vector4(0.108f, 0.082f, 0.11f, 1.0f));

		button->setSize(m_buttonSize);
		button->setText(std::to_string(digitIndex));
		button->setTextColor(vector3(1.0f, 1.0f, 1.0f));

		button->setTextFontSize(14);

		button->setPadding(uivector2(25, 15));

		button->onClick(std::bind(&CodePanel::digitButtonClickCallback, this, button, std::placeholders::_1));

		m_digitButtons[digitIndex] = button;
		addWidget(m_digitButtons[digitIndex]);
	}

	m_enterButton = new GUIButton(m_graphicsContext, m_textFont);
	m_enterButton->setSize(m_buttonSize);
	m_enterButton->setBackgroundColor(vector4(0.148f, 0.122f, 0.15f, 1.0f));
	m_enterButton->setHoverBackgroundColor(vector4(0.108f, 0.082f, 0.11f, 1.0f));
	m_enterButton->setText("OK");
	m_enterButton->setTextFontSize(12);
	m_enterButton->setPadding(uivector2(17, 15));
	m_enterButton->setTextColor(vector3(1.0f, 1.0f, 1.0f));
	m_enterButton->onClick(std::bind(&CodePanel::enterButtonClickCallback, this, std::placeholders::_1));
	addWidget(m_enterButton);

	m_clearButton = new GUIButton(m_graphicsContext, m_textFont);
	m_clearButton->setSize(m_buttonSize);
	m_clearButton->setBackgroundColor(vector4(0.148f, 0.122f, 0.15f, 1.0f));
	m_clearButton->setHoverBackgroundColor(vector4(0.108f, 0.082f, 0.11f, 1.0f));
	m_clearButton->setText("CLR");
	m_clearButton->setTextFontSize(12);
	m_clearButton->setPadding(uivector2(15, 15));
	m_clearButton->setTextColor(vector3(1.0f, 1.0f, 1.0f));
	m_clearButton->onClick(std::bind(&CodePanel::clearButtonClickCallback, this, std::placeholders::_1));
	addWidget(m_clearButton);

	m_codeTextBox = new GUITextBox(m_graphicsContext, m_textFont);
	m_codeTextBox->setSize(panelWidth - m_panelPadding.x * 2, 36);
	m_codeTextBox->setPaddingLeft(5);
	m_codeTextBox->setPaddingTop(5);
	m_codeTextBox->setBackgroundColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_codeTextBox->setColor(vector4(0.0f, 0.0f, 0.0f, 0.0f));
	m_codeTextBox->setFontSize(10);
	addWidget(m_codeTextBox);

	enableBackgroundRendering();
	setBackgroundColor(vector4(0.168f, 0.172f, 0.25f, 0.8f));
}

CodePanel::~CodePanel()
{
	delete m_clearButton;
	delete m_enterButton;
	delete m_codeTextBox;

	for (GUIButton* button : m_digitButtons)
		delete button;
}

void CodePanel::render(GeometryInstance * quad, GpuProgram * program)
{
	HUDWindow::render(quad, program);
}


void CodePanel::setCode(const std::string & code)
{
	m_code = code;
}

const std::string & CodePanel::getCode() const
{
	return m_code;
}

void CodePanel::setPosition(const uivector2 & position)
{
	HUDWindow::setPosition(position);

	updateLayout();
}

void CodePanel::setPosition(uint32 x, uint32 y)
{
	HUDWindow::setPosition(x, y);

	updateLayout();
}

void CodePanel::setSuccessulEnterCallback(const CodeEnterCallback & callback)
{
	m_successulEnterCallback = callback;
}

void CodePanel::updateLayout()
{
	m_codeTextBox->setPosition(m_position + m_panelPadding);

	for (size_t digitIndex = 1; digitIndex < 10; digitIndex++) {
		size_t rowIndex = (digitIndex - 1) / 3;
		size_t columnIndex = (digitIndex - 1) % 3;

		m_digitButtons[digitIndex]->setPosition(m_position + calculateButtonOffset(rowIndex, columnIndex));
	}

	m_digitButtons[0]->setPosition(m_position + calculateButtonOffset(3, 1));

	m_enterButton->setPosition(m_position + calculateButtonOffset(3, 2));
	m_clearButton->setPosition(m_position + calculateButtonOffset(3, 0));
}

uivector2 CodePanel::calculateButtonOffset(size_t rowIndex, size_t columnIndex) const
{
	unsigned int offsetX = m_panelPadding.x + m_buttonSize.x * columnIndex + m_buttonsOffset.x * columnIndex;
	unsigned int offsetY = m_codeTextBox->getHeight() + m_buttonsOffset.y +
		m_panelPadding.y + m_buttonSize.y * rowIndex + m_buttonsOffset.y * rowIndex;

	return uivector2(offsetX, offsetY);
}

void CodePanel::digitButtonClickCallback(GUIButton * digitButton, const MousePosition & position)
{
	m_codeTextBox->setText(m_codeTextBox->getText() + digitButton->getText());
}

void CodePanel::clearButtonClickCallback(const MousePosition & position)
{
	m_codeTextBox->clear();
}

void CodePanel::enterButtonClickCallback(const MousePosition & position)
{
	if (m_code == m_codeTextBox->getText()) {
		if (m_successulEnterCallback != nullptr)
			m_successulEnterCallback();
	}

	m_codeTextBox->clear();
	close();
}
