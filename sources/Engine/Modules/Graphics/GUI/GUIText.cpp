#include "precompiled.h"

#pragma hdrstop

#include "GUIText.h"
#include "GUISystem.h"

GUIText::GUIText(std::shared_ptr<BitmapFont> font, const std::string& text)
  : m_font(font),
    m_text(text),
    m_fontSize(font->getBaseSize())
{
  /* The widget size should not affect to vertices positions as
   * they are formed in real scale */
  disableScaleTransform();
}

void GUIText::setFont(std::shared_ptr<BitmapFont> font)
{
  m_font = font;
  resetTextGeometryCache();
}

std::shared_ptr<BitmapFont> GUIText::getFont() const
{
  return m_font;
}

void GUIText::setText(const std::string& text)
{
  m_text = text;
  resetTextGeometryCache();
}

std::string GUIText::getText() const
{
  return m_text;
}

void GUIText::setColor(const glm::vec4& color)
{
  setBackgroundColor(color);
}

glm::vec4 GUIText::getColor() const
{
  return getBackgroundColor();
}

void GUIText::setHoverColor(const glm::vec4& color)
{
  setHoverBackgroundColor(color);
}

glm::vec4 GUIText::getHoverColor() const
{
  return getHoverBackgroundColor();
}

void GUIText::setFontSize(int size)
{
  SW_ASSERT(size >= 0);

  m_fontSize = size;
  resetTextGeometryCache();
}

int GUIText::getFontSize() const
{
  return m_fontSize;
}

void GUIText::render(GUISystem& guiSystem)
{
  if (m_text.empty()) {
    return;
  }

  RenderTask task = guiSystem.getRenderTaskTemplate(this);

  task.geometryStore = updateAndGetGeometryStore();
  task.startOffset = 0;
  task.partsCount = task.geometryStore->getIndicesCount();

  GLShader* fragmentShader = task.material->getShadersPipeline()->getShader(GL_FRAGMENT_SHADER);
  fragmentShader->setParameter("widget.useColorAlphaTexture", true);
  fragmentShader->setParameter("widget.colorAlphaTexture", *m_font->getBitmap().get(), 1);

  guiSystem.getGraphicsContext()->executeRenderTask(task);
}

bool GUIText::isTextGeometryBufferOutdated() const
{
  return m_needTextGeometryUpdate;
}

GLGeometryStore* GUIText::updateAndGetGeometryStore()
{
  if (m_needTextGeometryUpdate) {
    m_textGeometryCache = std::unique_ptr<GLGeometryStore>(createStringGeometryBuffer(m_text));
    m_needTextGeometryUpdate = false;
  }

  return m_textGeometryCache.get();
}

void GUIText::resetTextGeometryCache()
{
  m_needTextGeometryUpdate = true;
}

GLGeometryStore* GUIText::createStringGeometryBuffer(const std::string& str)
{
  SW_ASSERT(str.size() > 0);

  std::vector<VertexPos3Norm3UV> vertices;
  std::vector<uint16_t> indices;

  int bitmapWidth = m_font->getBitmap()->getWidth();
  int bitmapHeight = m_font->getBitmap()->getHeight();

  int cursorPosition = 0;
  int cursorLineOffset = 0;

  int maxHeight = 0;

  for (size_t characterIndex = 0; characterIndex < str.size(); characterIndex++) {
    unsigned char character = static_cast<unsigned char>(str[characterIndex]);

    if (character == '\n') {
      cursorPosition = 0;
      cursorLineOffset += m_font->getHeight();

      continue;
    }

    BitmapCharacter characterDescription = m_font->getCharacter(character);
    glm::ivec2 atlasPosition = characterDescription.bitmapArea.getOrigin();
    glm::ivec2 characterSize = characterDescription.bitmapArea.getSize();

    glm::ivec2 originPosition(cursorPosition, cursorLineOffset);

    uint16_t verticesBaseSize = static_cast<uint16_t>(vertices.size());

    VertexPos3Norm3UV topLeftVertex{
      {
        originPosition.x + characterDescription.xOffset,
        originPosition.y + characterDescription.yOffset,
        0.0f
      },
      glm::vec3(0.0f),
      {
        static_cast<float>(atlasPosition.x) / bitmapWidth,
        static_cast<float>(atlasPosition.y) / bitmapHeight
      }
    };

    vertices.push_back(topLeftVertex);

    VertexPos3Norm3UV topRightVertex{
      {
        originPosition.x + characterSize.x + characterDescription.xOffset,
        originPosition.y + characterDescription.yOffset,
        0.0f
      },
      glm::vec3(0.0f),
      {
        static_cast<float>(atlasPosition.x + characterSize.x) / bitmapWidth,
        static_cast<float>(atlasPosition.y) / bitmapHeight
      }
    };

    vertices.push_back(topRightVertex);

    VertexPos3Norm3UV bottomRightVertex{
      {
        originPosition.x + characterSize.x + characterDescription.xOffset,
        originPosition.y + characterSize.y + characterDescription.yOffset,
        0.0f
      },
      glm::vec3(0.0f),
      {
        static_cast<float>(atlasPosition.x + characterSize.x) / bitmapWidth,
        static_cast<float>(atlasPosition.y + characterSize.y) / bitmapHeight
      }
    };

    vertices.push_back(bottomRightVertex);

    VertexPos3Norm3UV bottomLeftVertex{
      {
        originPosition.x + characterDescription.xOffset,
        originPosition.y + characterSize.y + characterDescription.yOffset,
        0.0f
      },
      glm::vec3(0.0f),
      {
        static_cast<float>(atlasPosition.x) / bitmapWidth,
        static_cast<float>(atlasPosition.y + characterSize.y) / bitmapHeight
      }
    };

    vertices.push_back(bottomLeftVertex);

    indices.push_back(verticesBaseSize + 1);
    indices.push_back(verticesBaseSize);
    indices.push_back(verticesBaseSize + 3);

    indices.push_back(verticesBaseSize + 2);
    indices.push_back(verticesBaseSize + 1);
    indices.push_back(verticesBaseSize + 3);

    cursorPosition += characterDescription.xAdvance;
  }

  float scaleFactor = static_cast<float>(m_fontSize) / m_font->getBaseSize();

  for (VertexPos3Norm3UV& vertex : vertices) {
    vertex.pos.x *= scaleFactor;
    vertex.pos.y *= scaleFactor;

    int vertexHeight = static_cast<int>(std::ceil(vertex.pos.y));
    maxHeight = std::max(vertexHeight, maxHeight);
  }

  setSize({cursorPosition, maxHeight});

  return new GLGeometryStore(vertices, indices);
}
