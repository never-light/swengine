#pragma once

#include <Engine\Components\GUI\GUIWidget.h>
#include <Engine\Components\GUI\GUILayout.h>
#include <Engine\Components\GUI\Widgets\GUIText.h>
#include <Engine\Components\GUI\Widgets\GUITextBox.h>
#include "Console.h"

#include <list>

class GUIConsoleWidget : public GUILayout {
public:
	GUIConsoleWidget(Console* console, Font* font, unsigned int viewportWidth, GraphicsContext* graphicsContext);
	~GUIConsoleWidget();

	void print(const std::string& message);
	void clear();

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;

	virtual void show() override;
	virtual void hide() override;
public:
	void setMaxTextLines(size_t maxTextLines);
	size_t getMaxLextLines() const;

	void setTextLineHeight(unsigned int height);
	unsigned int getTextLineHeight() const;

	void setPaddingTop(unsigned int padding);
	unsigned int getPaddingTop() const;

	void setPaddingLeft(unsigned int padding);
	unsigned int getPaddingLeft() const;

	void setTextBoxMargin(unsigned int margin);
	unsigned int getTextBoxMargin() const;

	void setFontSize(unsigned int size);
	unsigned int getFontSize() const;

	void setConsoleBackgroundColor(const vector4& color);
	vector4 getConsoleBackgroundColor() const;

	void setInputTextBoxBackgroundColor(const vector4& color);
	vector4 getInputTextBoxBackgroundColor() const;

	void setTextColor(const vector4& color);
	vector4 getTextColor() const;
	
	void setInputTextBoxColor(const vector4& color);
	vector4 getInputTextBoxColor() const;
private:
	void updateLayout();

private:
	void onConsoleInputKeyPress(Key key, KeyEvent::Modifier mod);
	void onConsolePrint(const std::string& text);

private:
	Console * m_console;

	std::list<GUIText*> m_textLines;
	GUITextBox* m_inputTextBox;

	Font* m_font;

	GraphicsContext* m_graphicsContext;

	unsigned int m_viewportWidth;

	size_t m_maxTextLines = 20;
	unsigned int m_textLineHeight = 20;
	unsigned int m_paddingLeft = 10;
	unsigned int m_paddingTop = 10;
	unsigned int m_textBoxMargin = 15;
	unsigned int m_fontSize = 9;

	vector4 m_consoleBackgroundColor = vector4(0.0f, 0.0f, 0.0f, 1.0f);
	vector4 m_textColor = vector4(1.0f, 1.0f, 1.0f, 1.0f);
};