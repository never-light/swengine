#include "GUIConsoleWidget.h"

GUIConsoleWidget::GUIConsoleWidget(Console* console, Font * font, unsigned int viewportWidth, GraphicsResourceFactory * graphicsResourceFactory, GraphicsContext * graphicsContext)
	: GUILayout(), 
	m_console(console),
	m_graphicsResourceFactory(graphicsResourceFactory), 
	m_graphicsContext(graphicsContext), 
	m_font(font),
	m_viewportWidth(viewportWidth)
{
	m_console->setPrintCallback(std::bind(&GUIConsoleWidget::onConsolePrint, this, std::placeholders::_1));
	
	m_inputTextBox = new GUITextBox(m_graphicsContext, m_graphicsResourceFactory, m_font);
	m_inputTextBox->setPaddingTop(6);

	m_inputTextBox->onKeyPress(std::bind(&GUIConsoleWidget::onConsoleInputKeyPress, this, std::placeholders::_1, std::placeholders::_2));

	addWidget(m_inputTextBox);

	for (const std::string& message : m_console->getMessagesHistory())
		onConsolePrint(message);
}

GUIConsoleWidget::~GUIConsoleWidget()
{
	delete m_inputTextBox;

	clear();
}

void GUIConsoleWidget::print(const std::string & message)
{
	m_console->print(message);
}

void GUIConsoleWidget::clear()
{
	for (GUIText* textLine : m_textLines) {
		this->removeWidget(textLine);
		delete textLine;
	}

	m_textLines.clear();
}

void GUIConsoleWidget::render(GeometryStore * quad, GpuProgram * program)
{ 
	matrix4 textArea;
	textArea = glm::translate(textArea, vector3(m_position, 0.0f));
	textArea = glm::scale(textArea, vector3(m_viewportWidth, m_paddingTop + m_maxTextLines * m_textLineHeight + m_textBoxMargin, 1.0f));

	program->setParameter("transform.localToWorld", textArea);
	program->setParameter("quad.color", m_backgroundColor);
	program->setParameter("quad.useTexture", false);

	quad->drawArrays(GeometryStore::DrawType::Triangles, 0, 6);

	for (GUIWidget* widget : m_widgets)
		widget->render(quad, program);
}

void GUIConsoleWidget::show()
{
	GUIWidget::show();

	if (m_onFocusCallback != nullptr)
		m_onFocusCallback(m_inputTextBox);
}

void GUIConsoleWidget::hide()
{
	GUIWidget::hide();

	if (m_onFocusCallback != nullptr)
		m_onFocusCallback(nullptr);
}

void GUIConsoleWidget::setMaxTextLines(size_t maxTextLines)
{
	m_maxTextLines = maxTextLines;

	if (maxTextLines <= m_textLines.size()) {
		for (GUIText* textLine : m_textLines)
			removeWidget(textLine);

		auto m_textLinesRemoveIt = m_textLines.begin();
		std::advance(m_textLinesRemoveIt, (m_textLines.size() - maxTextLines));

		m_textLines.erase(m_textLines.begin(), m_textLinesRemoveIt);
	}

	updateLayout();
}

unsigned int GUIConsoleWidget::getMaxLextLines() const
{
	return m_maxTextLines;
}

void GUIConsoleWidget::setTextLineHeight(unsigned int height)
{
	m_textLineHeight = height;

	updateLayout();
}

unsigned int GUIConsoleWidget::getTextLineHeight() const
{
	return m_textLineHeight;
}

void GUIConsoleWidget::setPaddingTop(unsigned int padding)
{
	m_paddingTop = padding;

	updateLayout();
}

unsigned int GUIConsoleWidget::getPaddingTop() const
{
	return m_paddingTop;
}

void GUIConsoleWidget::setPaddingLeft(unsigned int padding)
{
	m_paddingLeft = padding;

	m_inputTextBox->setPaddingLeft(padding);

	for (GUIText* textLine : m_textLines) {
		uivector2 currentTextLinePosition = textLine->getPosition();
		currentTextLinePosition.x = padding;

		textLine->setPosition(currentTextLinePosition);
	}
}

void GUIConsoleWidget::setTextBoxMargin(unsigned int margin)
{
	m_textBoxMargin = margin;
	updateLayout();
}

unsigned int GUIConsoleWidget::getTextBoxMargin() const
{
	return m_textBoxMargin;
}

void GUIConsoleWidget::setFontSize(unsigned int size)
{
	m_fontSize = size;

	m_inputTextBox->setFontSize(m_fontSize);

	for (GUIText* textLine : m_textLines)
		textLine->setFontSize(size);
}

unsigned int GUIConsoleWidget::getFontSize() const
{
	return m_fontSize;
}

void GUIConsoleWidget::setConsoleBackgroundColor(const vector4 & color)
{
	m_consoleBackgroundColor = color;
}

vector4 GUIConsoleWidget::getConsoleBackgroundColor() const
{
	return m_consoleBackgroundColor;
}

void GUIConsoleWidget::setInputTextBoxBackgroundColor(const vector4 & color)
{
	m_inputTextBox->setBackgroundColor(color);
}

vector4 GUIConsoleWidget::getInputTextBoxBackgroundColor() const
{
	return m_inputTextBox->getBackgroundColor();
}

void GUIConsoleWidget::setTextColor(const vector4 & color)
{
	m_textColor = color;

	for (GUIText* textLine : m_textLines)
		textLine->setColor(color);
}

vector4 GUIConsoleWidget::getTextColor() const
{
	return m_textColor;
}

void GUIConsoleWidget::setInputTextBoxColor(const vector4 & color)
{
	m_inputTextBox->setColor(color);
}

vector4 GUIConsoleWidget::getInputTextBoxColor() const
{
	return m_inputTextBox->getColor();
}

unsigned int GUIConsoleWidget::getPaddingLeft() const
{
	return m_paddingLeft;
}

void GUIConsoleWidget::updateLayout()
{
	m_inputTextBox->setSize(m_viewportWidth, 34);
	m_inputTextBox->setPosition(m_position + uivector2(0, m_paddingTop + m_maxTextLines * m_textLineHeight + m_textBoxMargin));

	size_t currentTextLineIndex = 0;

	for (GUIText* textLine : m_textLines) {
		uivector2 currentTextLinePosition = m_position + uivector2(m_paddingLeft, m_paddingTop + m_textLineHeight * currentTextLineIndex);

		textLine->setPosition(currentTextLinePosition);
		currentTextLineIndex++;
	}

	setSize(m_viewportWidth, m_paddingTop + m_maxTextLines * m_textLineHeight + m_textBoxMargin + m_inputTextBox->getSize().y);
}

void GUIConsoleWidget::onConsoleInputKeyPress(Key key, KeyEvent::Modifier mod)
{
	if (key == GLFW_KEY_ENTER) {
		m_console->executeCommand(m_inputTextBox->getText());
		m_inputTextBox->clear();
	}
	else if (key == GLFW_KEY_UP) {
		m_inputTextBox->setText(m_console->getLastCommand());
	}
}

void GUIConsoleWidget::onConsolePrint(const std::string & text) {
	if (m_textLines.size() == m_maxTextLines) {
		this->removeWidget(m_textLines.front());
		delete m_textLines.front(); 
		m_textLines.pop_front();

		for (GUIText* textLine : m_textLines)
			textLine->setPosition(textLine->getPosition() - uivector2(0, m_textLineHeight));
	}

	GUIText* textLine = new GUIText(m_graphicsResourceFactory);
	textLine->setFont(m_font);
	textLine->setFontSize(m_fontSize);
	textLine->setText(text);
	textLine->setColor(m_textColor);

	ivector2 linePosition;

	if (m_textLines.empty())
		linePosition = m_position + uivector2(m_paddingLeft, m_paddingTop);
	else
		linePosition = m_textLines.back()->getPosition() + uivector2(0, m_textLineHeight);

	textLine->setPosition(linePosition);
	m_textLines.push_back(textLine);

	this->addWidget(textLine);
}
