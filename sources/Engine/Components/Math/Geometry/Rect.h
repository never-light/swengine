#pragma once

#include <Engine\Components\Math\types.h>

class Rect {
public:
	Rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	Rect(const uivector2& position, const uivector2& size);
	Rect();

	~Rect();

	unsigned int getX() const;
	void setX(unsigned int x);

	unsigned int getY() const;
	void setY(unsigned int y);

	uivector2 getPosition() const;
	void setPosition(unsigned int x, unsigned int y);
	void setPosition(const uivector2& position);

	unsigned int getWidth() const;
	void setWidth(unsigned int width);

	unsigned int getHeight() const;
	void setHeight(unsigned int height);

	uivector2 getSize() const;
	void setSize(unsigned int width, unsigned int height);

private:
	uivector2 m_position;
	uivector2 m_size;
};