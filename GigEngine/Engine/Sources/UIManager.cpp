#include "UIManager.h"
#include "UIImage.h"
#include "UIText.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Mat4/FMat4.hpp"
#include "Vec3/FVec3.hpp"
#include "Application.h"
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
	auto* mainVertex = ResourceManager::Get<VertexShader>("Engine/Shaders/UIVertText.vert");
	auto* mainFragment = ResourceManager::Get<FragmentShader>("Engine/Shaders/UIFragText.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking UI shader" << std::endl;

	PROJECTIONLocation = GigRenderer::RENDERER.GetUniformLocation(shaderProgram.GetId(), "projection");
	COLORLocation = GigRenderer::RENDERER.GetUniformLocation(shaderProgram.GetId(), "textColor");
	IMAGELocation = GigRenderer::RENDERER.GetUniformLocation(shaderProgram.GetId(), "isImage");
}

void UIManager::AddImageElement()
{
	AddUIElement(new UIImage());
}

void UIManager::AddTextElement()
{
	elements.push_back(new UIText());
}

void UIManager::DrawUI()
{
	shaderProgram.Use();
	RENDERER.Disable(RD_DEPTH_TEST);

	RENDERER.SetUniformValue(PROJECTIONLocation, GigRenderer::UniformType::MAT4, &Application::GetWindow().GetOrthoMatrix());

	for (UIElement* elem : elements)
	{
		if (elem->IsActive())
		{
			RENDERER.SetUniformValue(IMAGELocation, GigRenderer::UniformType::INT, &elem->IsImage());
			RENDERER.SetUniformValue(COLORLocation, GigRenderer::UniformType::VEC3, &elem->GetColor());
			elem->Draw();
		}
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

UIElement* UIManager::CreateUIElement(UIElement* elem)
{
	const auto newElem = new UIElement(*elem);

	return AddUIElement(newElem);
}

UIElement* UIManager::AddUIElement(UIElement* elem)
{
	elements.push_back(elem);
	return elem;
}