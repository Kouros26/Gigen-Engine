#pragma once
#include "Displayable.h"

class GameObject;

class HierarchyDisplay : public Displayable
{
public:

	HierarchyDisplay();
	~HierarchyDisplay() override;

	void Draw() override;

private:

	void DisplayHierarchy();
	void DisplayGameObject(GameObject* obj, bool isChild);
};