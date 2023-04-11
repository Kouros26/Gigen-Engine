#include "Displayable.h"
#include "InterfaceManager.h"
#include "imgui.h"

Displayable::Displayable()
{
	width = g_basicSize;
	height = g_basicSize;
}

float Displayable::GetWidth()
{
	return width;
}

float Displayable::GetHeight()
{
	return height;
}

void Displayable::LimitWidthResize()
{
	ImVec2 size = ImGui::GetWindowSize();
	if (width != size.x)
	{
		width = size.x;
		if (width > (InterfaceManager::GetWidth() / 2))
		{
			width = InterfaceManager::GetWidth() / 2;
		}
	}
}

void Displayable::LimitHeightResize()
{
	ImVec2 size = ImGui::GetWindowSize();
	if (height != size.y)
	{
		height = size.y;
		if (height > (InterfaceManager::GetHeight() / 2))
		{
			height = InterfaceManager::GetHeight() / 2;
		}
	}
}