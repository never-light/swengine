#pragma once

#include <Engine/Components/Math/types.h>
#include <Engine/Components/Graphics/RenderSystem/GeometryInstance.h>
#include <Engine/Components/Graphics/RenderSystem/GpuProgram.h>
#include <Engine/Components/InputManager/Events.h>

class GUIWidget {
public:
	GUIWidget();
	virtual ~GUIWidget();

	virtual void render(GeometryInstance* quad, GpuProgram* program) = 0;
	virtual void update(const CursorPosition& mousePosition) = 0;

	virtual void onClick(const CursorPosition& mousePosition, MouseButton button);
	virtual void onHover(const CursorPosition& mousePosition);
	virtual void onMouseEnter(const CursorPosition& mousePosition);
	virtual void onMouseLeave(const CursorPosition& mousePosition);

	virtual void onKeyPress(KeyboardKey key);
	virtual void onKeyRepeat(KeyboardKey key);
	virtual void onCharacterEntered(unsigned char character);

	uivector2 getPosition() const;
	virtual void setPosition(const uivector2& position);
	virtual void setPosition(uint32 x, uint32 y);

	uivector2 getSize() const;
	void setSize(const uivector2& size);
	void setSize(uint32 x, uint32 y);

	void setWidth(uint32 width);
	uint32 getWidth() const;

	void setHeight(uint32 height);
	uint32 getHeight() const;

	bool isVisible() const;

	virtual void show();
	virtual void hide();

	void setZIndex(unsigned int index);
	unsigned int getZIndex() const;

	matrix4 getTransformationMatrix();
protected:
	uivector2 m_position;
	uivector2 m_size;

	bool m_visible;

	unsigned int m_zIndex;
};