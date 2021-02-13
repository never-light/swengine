#include "precompiled.h"

#pragma hdrstop

#include "GUIText.h"

#include <utility>

#include <utility>
#include "GUISystem.h"

GUIText::GUIText()
  : GUIWidget("label"),
    m_font(ResourceHandle<BitmapFont>()),
    m_fontSize(0)
{

}

GUIText::GUIText(ResourceHandle<BitmapFont> font, std::string text)
  : GUIWidget("label"),
    m_font(font),
    m_text(std::move(text)),
    m_fontSize(font->getBaseSize())
{
}

void GUIText::setFont(ResourceHandle<BitmapFont> font)
{
  m_font = font;
  resetTextGeometryCache();
}

ResourceHandle<BitmapFont> GUIText::getFont() const
{
  return m_font;
}

void GUIText::setText(const std::string& text)
{
  if (m_text != text) {
    m_text = text;
    resetTextGeometryCache();
  }
}

std::string GUIText::getText() const
{
  return m_text;
}

void GUIText::setColor(const glm::vec4& color, GUIWidgetVisualState visualState)
{
  getVisualParameters(visualState).setBackgroundColor(color);
}

glm::vec4 GUIText::getColor(GUIWidgetVisualState visualState) const
{
  return getVisualParameters(visualState).getBackgroundColor().value();
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

  auto& shadingParameters = dynamic_cast<ShadingParametersGUI&>(task.material->getParametersSet());
  shadingParameters.setAlphaTexture(m_font->getBitmapResource());

  task.mesh = updateAndGetGeometryStore();
  guiSystem.getGraphicsContext()->scheduleRenderTask(task);
}

Mesh* GUIText::updateAndGetGeometryStore()
{
  if (m_needTextGeometryUpdate) {
    auto geometryStoreParams = getStringGeometryStoreParams(m_text);

    GUIWidget::setSize(std::get<3>(geometryStoreParams));

    if (m_textGeometryCache == nullptr) {
      m_textGeometryCache = std::make_unique<Mesh>(true, MIN_TEXT_BLOCK_VERTICES_STORAGE_SIZE);

      auto& vertices = std::get<0>(geometryStoreParams);
      auto& uv = std::get<1>(geometryStoreParams);
      auto normals = std::vector<glm::vec3>(std::get<1>(geometryStoreParams).size());

      m_textGeometryCache->setVertices(vertices);
      m_textGeometryCache->setUV(uv);
      m_textGeometryCache->setNormals(normals);

      m_textGeometryCache->addSubMesh(std::get<2>(geometryStoreParams));
    }
    else {
      m_textGeometryCache->setVertices(std::get<0>(geometryStoreParams));
      m_textGeometryCache->setUV(std::get<1>(geometryStoreParams));
      m_textGeometryCache->setNormals(std::vector<glm::vec3>(std::get<1>(geometryStoreParams).size()));

      m_textGeometryCache->setIndices(std::get<2>(geometryStoreParams), 0);
    }

    m_needTextGeometryUpdate = false;
  }

  return m_textGeometryCache.get();
}

void GUIText::resetTextGeometryCache()
{
  m_needTextGeometryUpdate = true;
}

std::tuple<std::vector<glm::vec3>,
           std::vector<glm::vec2>,
           std::vector<uint16_t>, glm::ivec2> GUIText::getStringGeometryStoreParams(const std::string& str) const
{
  SW_ASSERT(m_font.get() != nullptr && "It is required to set font for the text line before rendering");
  SW_ASSERT(!str.empty() && "It is impossible to create geometry buffer for the empty text string");

  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> uv;
  std::vector<uint16_t> indices;

  int bitmapWidth = m_font->getBitmap()->getWidth();
  int bitmapHeight = m_font->getBitmap()->getHeight();

  int cursorPosition = 0;
  int cursorLineOffset = 0;

  int maxHeight = 0;

  for (char rawCharacter : str) {
    auto character = static_cast<unsigned char>(rawCharacter);

    if (character == '\n') {
      cursorPosition = 0;
      cursorLineOffset += m_font->getHeight();

      continue;
    }

    BitmapCharacter characterDescription = m_font->getCharacter(character);
    glm::ivec2 atlasPosition = characterDescription.bitmapArea.getOrigin();
    glm::ivec2 characterSize = characterDescription.bitmapArea.getSize();

    glm::ivec2 originPosition(cursorPosition, cursorLineOffset);

    auto verticesBaseSize = static_cast<uint16_t>(positions.size());

    positions.emplace_back(
      originPosition.x + characterDescription.xOffset,
      originPosition.y + characterDescription.yOffset,
      0.0f
    );

    uv.emplace_back(
      static_cast<float>(atlasPosition.x) / static_cast<float>(bitmapWidth),
      static_cast<float>(atlasPosition.y) / static_cast<float>(bitmapHeight)
    );

    positions.emplace_back(
      originPosition.x + characterSize.x + characterDescription.xOffset,
      originPosition.y + characterDescription.yOffset,
      0.0f
    );

    uv.emplace_back(
      static_cast<float>(atlasPosition.x + characterSize.x) / static_cast<float>(bitmapWidth),
      static_cast<float>(atlasPosition.y) / static_cast<float>(bitmapHeight)
    );

    positions.emplace_back(
      originPosition.x + characterSize.x + characterDescription.xOffset,
      originPosition.y + characterSize.y + characterDescription.yOffset,
      0.0f
    );

    uv.emplace_back(
      static_cast<float>(atlasPosition.x + characterSize.x) / static_cast<float>(bitmapWidth),
      static_cast<float>(atlasPosition.y + characterSize.y) / static_cast<float>(bitmapHeight)
    );

    positions.emplace_back(
      originPosition.x + characterDescription.xOffset,
      originPosition.y + characterSize.y + characterDescription.yOffset,
      0.0f
    );

    uv.emplace_back(
      static_cast<float>(atlasPosition.x) / static_cast<float>(bitmapWidth),
      static_cast<float>(atlasPosition.y + characterSize.y) / static_cast<float>(bitmapHeight)
    );

    indices.push_back(verticesBaseSize + 1);
    indices.push_back(verticesBaseSize);
    indices.push_back(verticesBaseSize + 3);

    indices.push_back(verticesBaseSize + 2);
    indices.push_back(verticesBaseSize + 1);
    indices.push_back(verticesBaseSize + 3);

    cursorPosition += characterDescription.xAdvance;
  }

  float scaleFactor = static_cast<float>(m_fontSize) / static_cast<float>(m_font->getBaseSize());

  for (glm::vec3& pos : positions) {
    pos.x *= scaleFactor;
    pos.y *= scaleFactor;

    int vertexHeight = static_cast<int>(std::ceil(pos.y));
    maxHeight = std::max(vertexHeight, maxHeight);
  }

  return {positions, uv, indices, {cursorPosition, maxHeight}};
}

[[nodiscard]] glm::mat4 GUIText::updateTransformationMatrix()
{
  /* The widget size should not affect to vertices positions as
   * they are formed in real scale */
  return glm::translate(glm::identity<glm::mat4x4>(),
    glm::vec3(getAbsoluteOrigin(), 0.0f));
}

void GUIText::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidget::applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "text-color") {
      // Text color
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) {
          ARG_UNUSED(arg);
          SW_ASSERT(false);
        },
        [this, visualState](const glm::vec4& color) {
          this->setColor(color, visualState);
        },
      }, property.getValue());
    }
    else if (propertyName == "text") {
      // Text
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) {
          ARG_UNUSED(arg);
          SW_ASSERT(false);
        },
        [this, visualState](const std::string& text) {
          SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Text is supported only for default state");

          this->setText(text);
        },
      }, property.getValue());
    }
    else if (propertyName == "font-size") {
      // Font size
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) {
          ARG_UNUSED(arg);
          SW_ASSERT(false);
        },
        [this, visualState](int size) {
          SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Font-size is supported only for default state");

          this->setFontSize(size);
        },
      }, property.getValue());
    }
    else if (propertyName == "font-family") {
      // Font family
      std::visit(GUIWidgetStylesheetPropertyVisitor{
        [](auto arg) {
          ARG_UNUSED(arg);
          SW_ASSERT(false);
        },
        [this, visualState](ResourceHandle<BitmapFont> font) {
          SW_ASSERT(visualState == GUIWidgetVisualState::Default && "Font-family is supported only for default state");

          this->setFont(font);
        },
      }, property.getValue());
    }
    else {
      SW_ASSERT(false);
    }
  });

  if (m_font.get() != nullptr && m_fontSize > 0 && !m_text.empty()) {
    RETURN_VALUE_UNUSED(updateAndGetGeometryStore());
  }
}

void GUIText::setSize(const glm::ivec2& size)
{
  ARG_UNUSED(size);
  SW_ASSERT(false && "It is forbidden to manually set size for GUIText widget");
}

glm::ivec2 GUIText::getSize() const
{
  if (m_needTextGeometryUpdate) {
    auto geometryStoreParams = getStringGeometryStoreParams(m_text);

    return std::get<3>(geometryStoreParams);
  }
  else {
    return GUIWidget::getSize();
  }
}

void GUIText::setWidth(int width)
{
  ARG_UNUSED(width);
  SW_ASSERT(false && "It is forbidden to manually set width for GUIText widget");
}

void GUIText::setHeight(int height)
{
  ARG_UNUSED(height);
  SW_ASSERT(false && "It is forbidden to manually set height for GUIText widget");
}
