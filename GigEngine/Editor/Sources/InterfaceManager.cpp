#include "InterfaceManager.h"
#include "Application.h"
#include "GameObjectInspector.h"
#include "FileDisplay.h"
#include "HierarchyDisplay.h"

void InterfaceManager::DrawEditor()
{
	width = (float)Application::GetWindow().GetWidth();
	height = (float)Application::GetWindow().GetHeight();

	for (const auto& displayable : displayables)
		displayable->Draw();

	float posX = GetClassWidth<HierarchyDisplay>();
	float posY = GetClassHeight<FileDisplay>();
	Application::GetWindow().SetViewPort((int)posX, (int)posY, (int)(width - GetClassWidth<GameObjectInspector>() - posX), (int)(height - posY));
}

void InterfaceManager::AddEditorElement(Displayable* element)
{
	displayables.push_back(element);
}

void InterfaceManager::Cleanup()
{
	for (const auto& displayable : displayables)
		delete displayable;
}

float InterfaceManager::GetWidth()
{
	return width;
}

float InterfaceManager::GetHeight()
{
	return height;
}