#include "InterfaceManager.h"

void InterfaceManager::DrawEditor()
{
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
