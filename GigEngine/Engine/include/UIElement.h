#pragma once
#include "RectTransform.h"

class UIElement
{
public:
	UIElement();
	UIElement(const std::string& name);
	~UIElement();

	virtual void Draw();

	void SetColor(lm::FVec3 c);

	lm::FVec3& GetColor();
	RectTransform& GetTransform();

	std::string GetName();
	unsigned int GetId();

private:
	bool isActive = true;
	std::string name{};
	unsigned int id{};

	lm::FVec3 color = lm::FVec3(1);
	RectTransform transform;

	//use so every UIElement has a different id
	inline static unsigned int uiElementIndex = 0;
};
