#include "UIElement.h"
#include "UIManager.h"

UIElement::UIElement()
{
	id = uiElementIndex;
	uiElementIndex++;
}

UIElement::UIElement(const std::string& name) : UIElement()
{
	this->name = name;
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

std::string UIElement::GetName()
{
	return name;
}

unsigned int UIElement::GetId()
{
	return id;
}