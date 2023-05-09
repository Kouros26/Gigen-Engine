#pragma once
#include "UIElement.h"
#include "Texture.h"
#include "Model.h"

const char* const g_defaultPlanePath = "Engine/Models/UI/Image.obj";

class UIImage : public UIElement
{
public:
	UIImage();
	~UIImage();

	virtual void Draw() override;

private:
	Texture* texture = nullptr;
	Model* model = nullptr;
};
