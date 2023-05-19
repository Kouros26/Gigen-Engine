#pragma once
#include "Displayable.h"

class GameObject;
class UIElement;

class HierarchyDisplay : public Displayable
{
public:

	HierarchyDisplay();
	~HierarchyDisplay() override;

	void Draw() override;

private:

	void DisplayHierarchy();

	void DisplayUI();
	void DisplayUIElement(UIElement& element);

	void CreatePopUp() const;

	void DisplayGameObject(GameObject& obj, bool isChild);
	void GameObjectClicked(GameObject& obj) const;
	void GameObjectPopUp(GameObject& obj) const;
};