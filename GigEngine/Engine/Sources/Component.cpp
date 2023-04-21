#include "Component.h"

Component::Component(GameObject* gameObject)
    :gameObject(gameObject)
{}

Component::~Component()
{
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::Awake()
{
}

void Component::LateUpdate()
{
}