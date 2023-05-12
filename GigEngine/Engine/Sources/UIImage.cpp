#include "UIImage.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace GigRenderer;

UIImage::UIImage() : UIElement("Image")
{
	model = ResourceManager::Get<Model>(g_defaultPlanePath);
	texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
}

UIImage::~UIImage()
{
}

void UIImage::SetTexture(const std::string& path)
{
	texture = ResourceManager::Get<Texture>(path);
}

void UIImage::Draw()
{
	shaderProgram.Use();
	RENDERER.SetUniformValue(MODELLocation, UniformType::MAT4, &GetTransform().getMatrix());
	RENDERER.SetUniformValue(COLORLocation, UniformType::VEC3, &GetColor());
	model->Draw(texture);
	shaderProgram.UnUse();
}

void UIImage::Init()
{
	auto* mainVertex = ResourceManager::Get<VertexShader>("Engine/Shaders/UIVert.vert");
	auto* mainFragment = ResourceManager::Get<FragmentShader>("Engine/Shaders/UIFrag.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking UI shader" << std::endl;

	MODELLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "model");
	COLORLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "textColor");
}