#include "RectTransform.h"

RectTransform::RectTransform()
{
	SetSize({ 0.1f,0.1f });
	SetPosition({ 0,0 });
	matrix = lm::FMat4::Transform({ position.x, position.y, 0 }, { 0, 0, rotation }, { scale.x, scale.y, 0 });
}

RectTransform::~RectTransform()
{
}

void RectTransform::SetPosition(lm::FVec2 vec)
{
	hasChanged = true;
	position = vec - lm::FVec2(1) + scale;
}

void RectTransform::SetWidth(float w)
{
	lm::FVec2 pos = GetPosition();
	scale.x = w;
	SetPosition(pos);
}

void RectTransform::SetHeight(float h)
{
	lm::FVec2 pos = GetPosition();
	scale.y = h;
	SetPosition(pos);
}

void RectTransform::SetSize(lm::FVec2 vec)
{
	lm::FVec2 pos = GetPosition();
	scale = vec;
	SetPosition(pos);
}

void RectTransform::SetRotation(float r)
{
	hasChanged = true;
	rotation = r;
	if (rotation > 360 || rotation < -360)
		rotation = std::fmod(rotation, 360);
}

lm::FVec2 RectTransform::GetPosition()
{
	lm::FVec2 v = position + lm::FVec2(1) - scale;
	return v;
}

lm::FVec2 RectTransform::GetSize()
{
	return scale;
}

float RectTransform::GetRotation()
{
	return rotation;
}

lm::FMat4& RectTransform::getMatrix()
{
	if (hasChanged) 
	{
		matrix = lm::FMat4::Transform({ position.x, position.y, 0 }, { rotation, 0, 0 }, { scale.x, scale.y, 0 });
		hasChanged = false;
	}

	return matrix;
}
