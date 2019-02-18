#pragma once

#include "HUDWindow.h"

#include <Engine\Components\GUI\Widgets\GUIText.h>
#include <Engine\Components\GUI\Widgets\GUITextBox.h>
#include <Engine\Components\GUI\Widgets\GUIButton.h>
#include <Engine\Components\GUI\Widgets\GUIImage.h>


class CodePanel : public HUDWindow {
public:
	using CodeEnterCallback = std::function<void()>;

public:
	CodePanel(GraphicsContext* graphicsContext, Font* textFont);
	~CodePanel();

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;

	void setCode(const std::string& code);
	const std::string& getCode() const;

	virtual void setPosition(const uivector2& position);
	virtual void setPosition(uint32 x, uint32 y);

	void setSuccessulEnterCallback(const CodeEnterCallback& callback);
private:
	void updateLayout();

	uivector2 calculateButtonOffset(size_t rowIndex, size_t columnIndex) const;

private:
	void digitButtonClickCallback(GUIButton* digitButton, const MousePosition& position);
	void clearButtonClickCallback(const MousePosition& position);
	void enterButtonClickCallback(const MousePosition& position);

private:
	std::string m_code;

private:
	CodeEnterCallback m_successulEnterCallback;

private:
	GraphicsContext * m_graphicsContext;
	Font* m_textFont;

	GUIButton* m_digitButtons[10];
	GUIButton* m_enterButton;
	GUIButton* m_clearButton;

	GUITextBox* m_codeTextBox;

	uivector2 m_buttonSize;
	uivector2 m_panelPadding;
	uivector2 m_buttonsOffset;
};