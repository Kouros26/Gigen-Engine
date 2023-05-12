#include "UIElement.h"
#include "UIManager.h"

UIElement::UIElement() : Object()
{
}

UIElement::UIElement(const std::string& name) : Object(name)
{
}

UIElement::UIElement(UIElement& other)
	: Object(other.GetName())
{
	transform = other.GetTransform();
	color = other.GetColor();
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

int& UIElement::IsImage()
{
	return isImage;
}

void UIElement::SetIsImage(int n)
{
	isImage = n;
}

lm::FVec3& UIElement::GetColor()
{
	return color;
}

RectTransform& UIElement::GetTransform()
{
	return transform;
}