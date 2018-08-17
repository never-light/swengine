#pragma once

#include <Engine\Components\Math\types.h>
#include <Engine\Components\Graphics\RenderSystem\GeometryStore.h>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\InputManager\InputEvents.h>

class GUIWidget {
public:
	GUIWidget();
	virtual ~GUIWidget();

	virtual void render(GeometryStore* quad, GpuProgram* program) = 0;
	virtual void update(const MousePosition& mousePosition) = 0;

	virtual void onClick(const MousePosition& mousePosition, MouseButton button);
	virtual void onHover(const MousePosition& mousePosition);
	virtual void onMouseEnter(const MousePosition& mousePosition);
	virtual void onMouseLeave(const MousePosition& mousePosition);

	virtual void onKeyPress(Key key, KeyEvent::Modifier mod);
	virtual void onKeyRepeat(Key key, KeyEvent::Modifier mod);
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