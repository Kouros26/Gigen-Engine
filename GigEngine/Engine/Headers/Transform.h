#pragma once
#include "Vec3/FVec3.hpp"
#include "Mat4/FMat4.hpp"

class Transform
{
public:
	Transform();
	~Transform();

	void SetPosition(lm::FVec3 pos);
	void SetRotation(lm::FVec3 rot);
	void SetScale(lm::FVec3 scl);

	lm::FVec3 GetPosition() const;
	lm::FVec3 GetScale() const;
	lm::FVec3 GetRotation() const;

	void AddScale(lm::FVec3 scl);
	void AddRotation(lm::FVec3 rot);
	void AddPosition(lm::FVec3 pos);

	lm::FVec3 GetFront();
	lm::FVec3 GetUp();
	lm::FVec3 GetRight();

	lm::FMat4 GetMatrix();

private:
	void UpdateMatrix();
	void LimitRotation();
	//

	lm::FVec3 scale = lm::FVec3(1);
	lm::FVec3 position;
	lm::FVec3 rotation;
	lm::FMat4 matrix;

	bool hasChanged = true;
};