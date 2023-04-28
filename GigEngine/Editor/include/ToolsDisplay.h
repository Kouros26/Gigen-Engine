#pragma once
#include "Displayable.h"

class ToolsDisplay : public Displayable
{
public:
	ToolsDisplay();
	~ToolsDisplay() override;

	void Draw() override;

private :
	void DrawExplorer();
	void DrawConsole();
};
