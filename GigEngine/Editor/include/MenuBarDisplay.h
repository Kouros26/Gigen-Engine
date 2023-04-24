#pragma once
#include "Displayable.h"

class MenuBarDisplay : public Displayable
{
public:
	MenuBarDisplay();
	~MenuBarDisplay() override;

	void Draw() override;
};
