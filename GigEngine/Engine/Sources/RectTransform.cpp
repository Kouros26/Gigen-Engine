#include "RectTransform.h"

RectTransform::RectTransform()
{
	SetPosition({ 0,0 });
	matrix = lm::FMat4::Transform({ position.x, position.y, 0 }, { 0 }, { scale.x, scale.y, 0 });
}

RectTransform::~RectTransform()
{
}

void RectTransform::SetPosition(lm::FVec2 vec)
{
	hasChanged = true;
	position = vec;
}

void RectTransform::SetWidth(float w)
{
	scale.x = w;
}

void RectTransform::SetHeight(float h)
{
	scale.y = h;
}

void RectTransform::SetSize(lm::FVec2 vec)
{
	scale = vec;
}

lm::FVec2 RectTransform::GetPosition()
{
	return position;
}

lm::FVec2 RectTransform::GetSize()
{
	return scale;
}

lm::FMat4& RectTransform::getMatrix()
{
	if (hasChanged)
	{
		matrix = lm::FMat4::Transform({ position.x, position.y, 0 }, { 0 }, { scale.x, scale.y, 0 });
		hasChanged = false;
	}

	return matrix;
}