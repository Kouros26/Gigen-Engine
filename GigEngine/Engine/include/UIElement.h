#pragma once
#include "Object.h"
#include "RectTransform.h"

class UIElement : public Object
{
public:
	UIElement();
	UIElement(const std::string& name);
	UIElement(UIElement& other);
	virtual ~UIElement() override;

	virtual void Draw();

	void SetColor(lm::FVec3 c);

	lm::FVec3& GetColor();
	RectTransform& GetTransform();
private:

	lm::FVec3 color = lm::FVec3(1);
	RectTransform transform;
};
