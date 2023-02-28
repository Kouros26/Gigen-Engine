#include "GameObject.h"
#include "Component.h"
#include "Model.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"

unsigned int GameObject::gameObjectIndex = 0;

GameObject::GameObject()
{
	gameObjectIndex++;
	id = gameObjectIndex;

	name = "GameObject " + id;
}

GameObject::GameObject(std::string name)
	:GameObject()
{
	if (!name.empty()) {
		this->name = name;
	}
}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i])
			delete components[i];
	}
	model = nullptr;
	GameObjectManager::Remove(this);
}

void GameObject::Destroy()
{
	this->~GameObject();
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::setModel(std::string const& filePath)
{
	model = ResourceManager::Get<Model>(filePath);
}

void GameObject::UpdateRender() const
{
	if (model)
		model->Draw();
}

void GameObject::UpdateComponents() const
{
	for (const auto& component : components)
	{
		component->Update();
	}
}

void GameObject::AddComponent(Component* newComponent)
{
	components.push_back(newComponent);
}