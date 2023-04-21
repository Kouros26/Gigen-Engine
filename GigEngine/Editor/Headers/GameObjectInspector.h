#pragma once
#include "Displayable.h"

class GameObject;
class Camera;

class GameObjectInspector : public Displayable
{
public:

	GameObjectInspector();
	~GameObjectInspector() override;

	void Draw() override;

private:
	void DrawGameObject();

	void DrawTransform(GameObject* pObject);
	void DrawModel(GameObject* pObject);

	void DrawSpecials(GameObject* pObject);
	void DrawComponents(GameObject* pObject);

	void DrawLight(GameObject* pObject);
	void DrawCamera(Camera* pObject);
};
