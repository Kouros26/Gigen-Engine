#include "UIManager.h"
#include "UIImage.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Mat4/FMat4.hpp"
#include "Vec3/FVec3.hpp"

using namespace GigRenderer;

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	for (UIElement* elem : elements)
	{
		delete elem;
	}
}

void UIManager::Init()
{
	auto* mainVertex = ResourceManager::Get<VertexShader>("Engine/Shaders/UIVert.vert");
	auto* mainFragment = ResourceManager::Get<FragmentShader>("Engine/Shaders/UIFrag.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking UI shader" << std::endl;

	MODELLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "model");
	COLORLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "textColor");;
}

void UIManager::AddImageElement()
{
	elements.push_back(new UIImage());
}

void UIManager::AddTextElement()
{
	//elements.push_back(UIImage());
}

void UIManager::DrawUI()
{
	shaderProgram.Use();

	RENDERER.Disable(RD_DEPTH_TEST);

	for (UIElement* elem : elements)
	{
		RENDERER.SetUniformValue(MODELLocation, UniformType::MAT4, &elem->GetTransform().getMatrix());

		RENDERER.SetUniformValue(COLORLocation, UniformType::VEC3, &elem->GetColor());
		
		elem->Draw();
	}

	shaderProgram.UnUse();
}
