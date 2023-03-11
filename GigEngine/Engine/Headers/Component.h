#pragma once
#include "Watch.h"
#include "GameObject.h"

class Component
{
public:
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void Start();
	virtual void Update();

protected:
	GameObject* gameObject;
};

class Script : public Component
{
public:
	Script(GameObject* obj) : Component(obj) {}

	virtual void Awake();
	virtual void LateUpdate();
};

class TestScript : public Script
{
public:
	TestScript(GameObject* obj) : Script(obj) {}

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
};

class TestComponent : public Component
{
public:
	TestComponent(GameObject* obj) : Component(obj) {}

	virtual void Update() override {
		gameObject->GetTransform().AddPosition(lm::FVec3(0, 0.5f, 0.f) * Time::GetDeltaTime());
	};
};

class testComponent2 : public Component
{
public:
	testComponent2(GameObject* obj) : Component(obj) {};

	virtual void Update() override {
		gameObject->GetTransform().AddRotation(lm::FVec3(0, 0, 20.f) * Time::GetDeltaTime());
	};
};