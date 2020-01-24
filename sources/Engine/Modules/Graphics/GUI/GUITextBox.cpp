#include "precompiled.h"
#pragma hdrstop

#include "GUITextBox.h"
#include "GUISystem.h"

GUITextBox::GUITextBox(std::shared_ptr<BitmapFont> font)
    : m_font(font),
      m_text(std::make_unique<GUIText>(font, ""))
{
}

void GUITextBox::setFont(std::shared_ptr<BitmapFont> font)
{
    m_font = font;
}

std::shared_ptr<BitmapFont> GUITextBox::getFont() const
{
    return m_font;
}

void GUITextBox::setText(const std::string& text)
{
    m_text->setText(text);
}

std::string GUITextBox::getText() const
{
    return m_text->getText();
}

void GUITextBox::setTextColor(const glm::vec4& color)
{
    m_text->setColor(color);
}

glm::vec4 GUITextBox::getTextColor() const
{
    return m_text->getColor();
}

void GUITextBox::setTextHoverColor(const glm::vec4& color)
{
    m_text->setHoverColor(color);
}

glm::vec4 GUITextBox::getTextHoverColor() const
{
    return m_text->getHoverColor();
}

void GUITextBox::setTextFontSize(int size)
{
    m_text->setFontSize(size);
}

int GUITextBox::getTextFontSize() const
{
    return m_text->getFontSize();
}

void GUITextBox::render(GUISystem& guiSystem)
{
    RenderTask task = guiSystem.getRenderTaskTemplate(this);
    guiSystem.getGraphicsContext()->executeRenderTask(task);

    m_text->render(guiSystem);
}

bool GUITextBox::canHaveFocus() const
{
    return true;
}

void GUITextBox::processKeyboardEvent(const GUIKeyboardEvent& event)
{
    if (event.type != KeyboardEventType::KeyDown) {
        return;
    }

    char character = static_cast<char>(event.keyCode);

    if (event.keyCode == SDLK_BACKSPACE) {
        std::string text = m_text->getText();

        if (!text.empty()) {
            text.pop_back();
            m_text->setText(text);
        }
    }
    else if (isprint(character)) {
        char printedCharacter = character;

        if (event.keyModifiers & KMOD_SHIFT) {
            printedCharacter = static_cast<char>(std::toupper(printedCharacter));
        }

        m_text->setText(m_text->getText() + printedCharacter);
    }
}

void GUITextBox::transformationCacheUpdate()
{
    if (m_text->getParent() == nullptr) {
        addChildWidget(m_text);
    }

    if (!m_text->getText().empty()) {
        if (m_text->m_needTextGeometryUpdate) {
            m_text->updateAndGetGeometryStore();
        }

        m_text->setOrigin({ 10, getSize().y / 2 - m_text->getSize().y / 2 });
    }
}
