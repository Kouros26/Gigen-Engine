#pragma once
#include "Displayable.h"
#include "Vec3/FVec3.hpp"
#include <string>

class GameObject;
class Camera;
class RigidBody;

class GameObjectInspector : public Displayable
{
public:

	GameObjectInspector();
	~GameObjectInspector() override;

	void Draw() override;

private:
	void DrawGameObject();

	void DrawTransform(GameObject* pObject) const;
	void DrawModel(GameObject* pObject) const;
	void DrawTexture(GameObject* pObject) const;
	void DrawRigidBody(GameObject* pObject) const;
	void DrawRigidShape(RigidBody* body) const;
	void DrawRigidTransform(RigidBody* body) const;

	void DrawSpecials(GameObject* pObject) const;
	void DrawComponents(GameObject* pObject) const;

	void DrawLight(GameObject* pObject) const;
	void DrawCamera(Camera* pObject) const;

	void DrawAddComponent(GameObject* pObject) const;

	void DrawDropTarget(GameObject* pObject) const;

	void LockCalculation(float* fvec3, const lm::FVec3& original) const;

	static std::string GetFilePathFromExplorer(const char* filter);
};
