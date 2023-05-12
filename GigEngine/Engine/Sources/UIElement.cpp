#include "UIElement.h"
#include "UIManager.h"

UIElement::UIElement() : Object()
{
}

UIElement::UIElement(const std::string& name) : Object(name)
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