#pragma once
#include "Displayable.h"

class GameObject;

class HierarchyDisplay : public Displayable
{
public:

	HierarchyDisplay();
	~HierarchyDisplay() override;

	void Draw() override;

private :
	int node_clicked = -1;

	void DisplayHierarchy();
	void DisplayGameObject(GameObject* obj, bool isChild);
};