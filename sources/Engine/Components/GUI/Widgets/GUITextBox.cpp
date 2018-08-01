#include "GUITextBox.h"

GUITextBox::GUITextBox(GraphicsContext* graphicsContext, GraphicsResourceFactory * graphicsResourceFactory, Font * font)
	: m_text(new GUIText(graphicsResourceFactory)),
	m_graphicsContext(graphicsContext),
	m_paddingTop(0), m_paddingLeft(0), m_backgroundColor(1.0, 1.0, 1.0, 1.0),
	m_keyPressCallback(nullptr)
{
	setFont(font);
	setText("");
}

GUITextBox::~GUITextBox()
{
	if (m_text != nullptr) {
		delete m_text;
		m_text = nullptr;
	}
}

void GUITextBox::setText(const std::string & text)
{
	m_text->setText(text);
}

std::string GUITextBox::getText() const
{
	return m_text->getText();
}

void GUITextBox::clear()
{
	setText("");
}

void GUITextBox::setColor(const vector3 & color)
{
	m_text->setColor(color);
}

void GUITextBox::setColor(float r, float g, float b)
{
	m_text->setColor(r, g, b);
}

vector4 GUITextBox::getColor() const
{
	return m_text->getColor();
}

void GUITextBox::setColor(const vector4 & color)
{
	m_text->setColor(color);
}

void GUITextBox::setColor(float r, float g, float b, float a)
{
	m_text->setColor(r, g, b, a);
}

void GUITextBox::setFont(Font * font)
{
	m_text->setFont(font);
}

Font * GUITextBox::getFont() const
{
	return m_text->getFont();
}

void GUITextBox::setFontSize(unsigned int size)
{
	m_text->setFontSize(size);
}

unsigned int GUITextBox::getFontSize() const
{
	return m_text->getFontSize();
}

void GUITextBox::setPaddingTop(unsigned int paddingTop)
{
	m_paddingTop = paddingTop;
	m_text->setPosition(m_position + uivector2(m_paddingLeft, m_paddingTop));
}

unsigned int GUITextBox::getPaddingTop() const
{
	return m_paddingTop;
}

void GUITextBox::setPaddingLeft(unsigned int paddingLeft)
{
	m_paddingLeft = paddingLeft;
	m_text->setPosition(m_position + uivector2(m_paddingLeft, m_paddingTop));
}

unsigned int GUITextBox::getPaddingLeft() const
{
	return m_paddingLeft;
}

void GUITextBox::setPosition(const uivector2 & position)
{
	GUIWidget::setPosition(position);
	m_text->setPosition(m_position + uivector2(m_paddingLeft, m_paddingTop));
}

void GUITextBox::setPosition(uint32 x, uint32 y)
{
	GUIWidget::setPosition(x, y);
	m_text->setPosition(m_position + uivector2(m_paddingLeft, m_paddingTop));
}

void GUITextBox::render(GeometryStore * quad, GpuProgram * program)
{
	program->setParameter("transform.localToWorld", getTransformationMatrix());
	program->setParameter("quad.color", m_backgroundColor);
	program->setParameter("quad.useTexture", false);

	quad->drawArrays(GeometryStore::DrawType::Triangles, 0, 6);

	m_graphicsContext->enableScissorTest();
	m_graphicsContext->setScissorRectangle(Rect(m_position, m_size));
	m_text->render(quad, program);
	m_graphicsContext->disableScissorTest();
}

void GUITextBox::update(const MousePosition & mousePosition)
{
}

void GUITextBox::setBackgroundColor(const vector4 & color)
{
	m_backgroundColor = color;
}

void GUITextBox::setBackgroundColor(float r, float g, float b, float a)
{
	m_backgroundColor.r = r;
	m_backgroundColor.g = g;
	m_backgroundColor.b = b;
	m_backgroundColor.a = a;
}

vector4 GUITextBox::getBackgroundColor() const
{
	return m_backgroundColor;
}

void GUITextBox::onKeyPress(Key key, KeyEvent::Modifier mod)
{
	if (key == GLFW_KEY_BACKSPACE) {
		m_text->removeLastCharacter();
	}

	if (m_keyPressCallback != nullptr) {
		m_keyPressCallback(key, mod);
	}
}

void GUITextBox::onKeyRepeat(Key key, KeyEvent::Modifier mod)
{
	if (key == GLFW_KEY_BACKSPACE) {
		m_text->removeLastCharacter();
	}
}

void GUITextBox::onCharacterEntered(unsigned char character)
{
	m_text->appendCharacter(character);
}

void GUITextBox::onKeyPress(const KeyPressCallback & callback)
{
	m_keyPressCallback = callback;
}
