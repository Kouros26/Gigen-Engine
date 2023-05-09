#include "UIElement.h"
#include "UIManager.h"

UIElement::UIElement()
{
}

UIElement::~UIElement()
{
}

void UIElement::Draw()
{
}

void UIElement::SetColor(lm::FVec3 c)
{
	color = c;
}

lm::FVec3& UIElement::GetColor()
{
	return color;
}

RectTransform& UIElement::GetTransform()
{
	return transform;
}
