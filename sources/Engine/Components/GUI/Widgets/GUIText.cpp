#include "GUIText.h"

#include <algorithm>

#include <Engine\assertions.h>

GUIText::GUIText(GraphicsResourceFactory * graphicsResourceFactory)
	: m_graphicsResourceFactory(graphicsResourceFactory), 
	m_font(nullptr), 
	m_textGeometry(nullptr),
	m_textGeometryVerticesCount(0),
	m_text(),
	m_fontSize(0),
	m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

GUIText::~GUIText()
{
	if (m_textGeometry != nullptr) {
		delete m_textGeometry;
		m_textGeometry = nullptr;
	}
}

void GUIText::setText(const std::string & text)
{
	m_text = text;

	if (m_font != nullptr)
		updateTextGeometry();
}

std::string GUIText::getText() const
{
	return m_text;
}

void GUIText::appendCharacter(unsigned char character)
{
	m_text.push_back(character);

	if (m_font != nullptr)
		updateTextGeometry();
}

void GUIText::removeLastCharacter()
{
	if (m_text.empty())
		return;

	m_text.pop_back();

	if (m_font != nullptr)
		updateTextGeometry();
}

void GUIText::setColor(const vector3 & color)
{
	m_color = vector4(color, 1.0f);
}

void GUIText::setColor(float r, float g, float b)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
}

void GUIText::setColor(const vector4 & color)
{
	m_color = color;
}

void GUIText::setColor(float r, float g, float b, float a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

vector4 GUIText::getColor() const
{
	return m_color;
}

void GUIText::setFont(Font * font)
{
	_assert(font != nullptr);

	m_font = font;
	m_fontSize = font->getBaseSize();

	if (!m_text.empty())
		updateTextGeometry();
}

Font * GUIText::getFont() const
{
	return m_font;
}

void GUIText::setFontSize(unsigned int size)
{
	m_fontSize = size;
}

unsigned int GUIText::getFontSize() const
{
	return m_fontSize;
}

void GUIText::render(GeometryStore * quad, GpuProgram * program)
{
	m_font->getBitmap()->bind(0);

	matrix4 textTransformation = glm::translate(vector3(m_position, 0.0));

	program->setParameter("transform.localToWorld", textTransformation);

	program->setParameter("quad.color", m_color);
	program->setParameter("quad.texture", 0);
	program->setParameter("quad.useTexture", true);
	program->setParameter("quad.useFirstChannel", true);

	m_textGeometry->bind();
	m_textGeometry->drawArrays(GeometryStore::DrawType::Triangles, 0, m_textGeometryVerticesCount);

	quad->bind();
}

void GUIText::update(const MousePosition & mousePosition)
{
}
 
void GUIText::updateTextGeometry()
{
	std::vector<vector4> vertices;
	std::vector<unsigned int> indices;

	unsigned int bitmapWidth = m_font->getBitmap()->getWidth();
	unsigned int bitmapHeight = m_font->getBitmap()->getHeight();

	unsigned int cursorPosition = 0;
	unsigned int maxHeight = 0;

	for (unsigned char character : m_text) {
		Character characterDescription = m_font->getCharacter(character);
		uivector2 atlasPosition = characterDescription.rectangleInAtlas.getPosition();
		uivector2 characterSize = characterDescription.rectangleInAtlas.getSize();

		ivector2 originPosition(cursorPosition, 0);

		vector4 topLeftVertex(
			originPosition.x + characterDescription.xOffset, 
			originPosition.y + characterDescription.yOffset,
			(float)atlasPosition.x / bitmapWidth,
			(float)(atlasPosition.y) / bitmapHeight);

		vector4 topRightVertex(
			originPosition.x + characterSize.x + characterDescription.xOffset,
			originPosition.y + characterDescription.yOffset,
			(float)(atlasPosition.x + characterSize.x) / bitmapWidth,
			(float)(atlasPosition.y) / bitmapHeight);

		vector4 bottomRightVertex(
			originPosition.x + characterSize.x + characterDescription.xOffset,
			originPosition.y + characterSize.y + characterDescription.yOffset,
			(float)(atlasPosition.x + characterSize.x) / bitmapWidth,
			(float)(atlasPosition.y + characterSize.y) / bitmapHeight);

		vector4 bottomLeftVertex(
			originPosition.x + characterDescription.xOffset,
			originPosition.y + characterSize.y + characterDescription.yOffset,
			(float)(atlasPosition.x) / bitmapWidth,
			(float)(atlasPosition.y + characterSize.y) / bitmapHeight);

		vertices.push_back(topRightVertex);
		vertices.push_back(topLeftVertex);
		vertices.push_back(bottomLeftVertex);

		vertices.push_back(bottomRightVertex);
		vertices.push_back(topRightVertex);
		vertices.push_back(bottomLeftVertex);

		cursorPosition += characterDescription.xAdvance;
		
		maxHeight = std::max(characterSize.y, maxHeight);
	}

	float scaleFactor = (float)m_fontSize / m_font->getBaseSize();

	for (vector4& vertex : vertices) {
		vertex.x *= scaleFactor;
		vertex.y *= scaleFactor;
	}

	if (m_textGeometry == nullptr)
		m_textGeometry = m_graphicsResourceFactory->createGeometryStore();
	else
		m_textGeometry->destroy();

	GeometryStore::BufferId vertexBufferId = m_textGeometry->requireBuffer(GeometryStore::BufferType::Vertex, GeometryStore::BufferUsage::StaticDraw, vertices.size() * sizeof(vertices[0]));
	m_textGeometry->setBufferData(vertexBufferId, 0, vertices.size() * sizeof(vertices[0]), (const std::byte*)vertices.data());

	// position and texture coordinates attribute
	m_textGeometry->setVertexLayoutAttribute(0, vertexBufferId, 4,
		GeometryStore::VertexLayoutAttributeBaseType::Float, false, 4 * sizeof(float), 0);

	m_textGeometry->create();

	m_textGeometryVerticesCount = vertices.size();
	setSize(cursorPosition, maxHeight);
}
