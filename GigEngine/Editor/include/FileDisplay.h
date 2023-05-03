#pragma once
#include "Displayable.h"
#include <string>

class FileDisplay : public Displayable
{
public:
	FileDisplay();
	~FileDisplay() override;

	void Draw() override;

private:
	std::string root = "Assets";

	void DrawFolders(std::string path);
};
