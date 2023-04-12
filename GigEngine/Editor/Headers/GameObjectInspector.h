#pragma once
#include "Displayable.h"

class GameObject;

class GameObjectInspector : public Displayable
{
public:

	GameObjectInspector();
	~GameObjectInspector() override;

	void Draw() override;

private:
	void DrawGameObject();
	void DrawTransform(GameObject* object);
};
