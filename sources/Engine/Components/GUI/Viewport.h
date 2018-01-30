#pragma once

class Viewport {
public:
	Viewport(int width, int height);
	~Viewport();

	int getWidth() const;
	int getHeight() const;

	void setWidth(int width);
	void setHeight(int height);

private:
	int m_width;
	int m_height;
};