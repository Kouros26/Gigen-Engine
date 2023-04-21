#pragma once

class Displayable
{
public:
	Displayable();
	virtual ~Displayable() = default;

	virtual void Draw() = 0;

	float GetWidth();
	float GetHeight();

protected:
	void LimitWidthResize();
	void LimitHeightResize();

	float width;
	float height;
};
