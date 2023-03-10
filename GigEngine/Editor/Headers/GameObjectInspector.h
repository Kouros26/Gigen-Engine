#pragma once
#include "Displayable.h"
#include "GameObject.h"
#include "GameObjectManager.h"

class GameObjectInspector : public Displayable
{
public:

	GameObjectInspector();
	~GameObjectInspector() override;

	void Draw() override;

private:

	void GetGameObjects();
};
