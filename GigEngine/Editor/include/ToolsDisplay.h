#pragma once
#include "Displayable.h"
#include <string>

class ToolsDisplay : public Displayable
{
public:
	ToolsDisplay();
	~ToolsDisplay() override;

	void Draw() override;

private:
	const std::string rootDirPath = "../../../Resources";
	std::string currentDirPath;
	const float padding = 16.0f;
	const float cellSize = 100.0f;

	void DrawFiles(const std::string& path);

	void DrawExplorer();
	void DrawConsole();
};
