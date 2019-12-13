#pragma once

#include "GUIWidget.h"
#include "BitmapFont.h"
#include "GUIText.h"

class GUITextBox : public GUIWidget
{
public:
    GUITextBox(std::shared_ptr<BitmapFont> font);

    void setFont(std::shared_ptr<BitmapFont> font);
    std::shared_ptr<BitmapFont> getFont() const;

    void setText(const std::string& text);
    std::string getText() const;

    void setTextColor(const glm::vec4& color);
    glm::vec4 getTextColor() const;

    void setTextHoverColor(const glm::vec4& color);
    glm::vec4 getTextHoverColor() const;

    void setTextFontSize(int size);
    int getTextFontSize() const;

    void render(GUISystem &guiSystem) override;

    bool canHaveFocus() const override;

protected:
    void processKeyboardEvent(const GUIKeyboardEvent &event) override;

    void transformationCacheUpdate() override;

private:
    std::shared_ptr<BitmapFont> m_font;
    std::shared_ptr<GUIText> m_text;
};

