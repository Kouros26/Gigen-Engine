#pragma once
#include <vector>
#include "Displayable.h"

class InterfaceManager
{
public:

	static void DrawEditor();
	static void AddEditorElement(Displayable* element);
	static void Cleanup();

private:

	inline static std::vector<Displayable*> displayables{};
};
