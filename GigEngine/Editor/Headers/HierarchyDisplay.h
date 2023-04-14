#pragma once
#include "Displayable.h"

class HierarchyDisplay : public Displayable
{
public:

	HierarchyDisplay();
	~HierarchyDisplay() override;

	void Draw() override;
};