#pragma once

#include "Modules/Graphics/OpenGL/GLGeometryStore.h"
#include "GUIWidget.h"
#include "BitmapFont.h"

class GUITextBox;

class GUIText : public GUIWidget
{
public:
    GUIText(std::shared_ptr<BitmapFont> font, const std::string& text);

    void setFont(std::shared_ptr<BitmapFont> font);
    std::shared_ptr<BitmapFont> getFont() const;

    void setText(const std::string& text);
    std::string getText() const;

    void setColor(const glm::vec4& color);
    glm::vec4 getColor() const;

    void setHoverColor(const glm::vec4& color);
    glm::vec4 getHoverColor() const;

    void setFontSize(int size);
    int getFontSize() const;

    void render(GUISystem &guiSystem) override;

protected:
    bool isTextGeometryBufferOutdated() const;

private:
    void resetTextGeometryCache();

    const GLGeometryStore* updateAndGetGeometryStore();
    GLGeometryStore* createStringGeometryBuffer(const std::string& str);

private:
    std::shared_ptr<BitmapFont> m_font;

    std::string m_text;

    int m_fontSize;

    mutable std::unique_ptr<GLGeometryStore> m_textGeometryCache;
    mutable bool m_needTextGeometryUpdate = true;

private:
    friend class GUITextBox;
};

