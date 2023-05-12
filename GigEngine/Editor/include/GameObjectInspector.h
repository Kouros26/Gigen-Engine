#pragma once
#include "Displayable.h"
#include <string>

class GameObject;
class UIElement;
class UIImage;
class UIText;
class Camera;
class RigidBody;

class GameObjectInspector : public Displayable
{
public:

	GameObjectInspector();
	~GameObjectInspector() override;

	void Draw() override;

private:
	void DrawObject();
	void DrawGameObject(GameObject* pObject) const;

	void DrawUIElement(UIElement* pUI) const;
	void DrawTransform2D(UIElement* pUI) const;
	void DrawDropTargetImage(UIImage* pImage) const;
	void DrawUIText(UIText* pText) const;

	void DrawTransform(GameObject* pObject) const;
	void DrawModel(GameObject* pObject) const;
	void DrawTexture(GameObject* pObject) const;
	void DrawRigidBody(GameObject* pObject) const;
	void DrawRigidShape(RigidBody* body) const;

	void DrawSpecials(GameObject* pObject) const;
	void DrawComponents(GameObject* pObject) const;

	void DrawLight(GameObject* pObject) const;
	void DrawCamera(Camera* pObject) const;

	void DrawAddComponent(GameObject* pObject) const;

	void DrawDropTarget(GameObject* pObject) const;

	static std::string GetFilePathFromExplorer(const char* filter);
};
