#pragma once
#include "UIElement.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

const char* const g_defaultPlanePath = "Engine/Models/UI/Image.obj";

class UIImage : public UIElement
{
public:
	UIImage();
	~UIImage();

	virtual void SetTexture(const std::string& path);
	virtual void Draw() override;

	static void Init();

private:
	inline static unsigned int MODELLocation;
	inline static unsigned int COLORLocation;
	inline static ShaderProgram shaderProgram;

	Texture* texture = nullptr;
	Model* model = nullptr;
};
