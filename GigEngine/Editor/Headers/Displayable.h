#pragma once

class Displayable
{
public:

	virtual ~Displayable()= default;

	virtual void Draw() = 0;
};
