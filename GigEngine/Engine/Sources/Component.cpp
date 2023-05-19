#include "Component.h"

Component::Component(GameObject* gameObject)
    :gameObject(gameObject)
{}

GameObject* Component::GetGameObject() const
{
    return gameObject;
}

void Component::Update(float pDeltaTime)
{
}

void Component::LateUpdate(float pDeltaTime)
{
}

Component::~Component()
{
}

void Component::Start()
{
}

void Component::Awake()
{
}