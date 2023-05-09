#pragma once
#include "RectTransform.h"

class UIElement
{
public:
	UIElement();
	~UIElement();

	virtual void Draw();

	void SetColor(lm::FVec3 c);

	lm::FVec3& GetColor();
	RectTransform& GetTransform();

private:
	lm::FVec3 color = lm::FVec3(1);
	RectTransform transform;
};
