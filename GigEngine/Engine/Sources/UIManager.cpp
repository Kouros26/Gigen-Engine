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
	UIText::Init();
	UIImage::Init();
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
	RENDERER.Disable(RD_DEPTH_TEST);

	for (UIElement* elem : elements)
	{
		if (elem->IsActive())
		{
			elem->Draw();
		}
	}
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