#include "UIManager.h"
#include "UIImage.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Mat4/FMat4.hpp"
#include "Vec3/FVec3.hpp"
#include "GameObjectManager.h"

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
	UIImage* img = new UIImage();
	img->SetTexture("Engine/Textures/ok.png");
	img->SetColor(lm::FVec3(1, 1, 0));
	elements.push_back(img);
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

int UIManager::GetSize()
{
	return elements.size();
}

UIElement* UIManager::GetElement(unsigned int i)
{
	if (i < GetSize())
	{
		return elements.at(i);
	}
	return nullptr;
}

void UIManager::SetFocusedElement(UIElement* elem)
{
	focusedElement = elem;
	if (elem)
		GameObjectManager::SetFocusedGameObject(nullptr);
}

UIElement* UIManager::GetFocusedElement()
{
	return focusedElement;
}

void UIManager::RemoveElement(UIElement* elem)
{
	const auto it = std::ranges::find(elements, elem);

	if (it == elements.end())
		return;

	elements.erase(it);

	elem->~UIElement();
}