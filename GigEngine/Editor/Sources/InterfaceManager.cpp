#include "InterfaceManager.h"
#include "Application.h"

void InterfaceManager::DrawEditor()
{
	width = (float)Application::GetWindow().GetWidth();
	height = (float)Application::GetWindow().GetHeight();

	for (const auto& displayable : displayables)
		displayable->Draw();
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