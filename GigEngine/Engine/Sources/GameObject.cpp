#include "GameObject.h"
#include "Component.h"
#include "Model.h"
#include "ResourceManager.h"

unsigned int GameObject::gameObjectIndex = 0;

GameObject::GameObject()
{
	gameObjectIndex++;
	id = gameObjectIndex;

	name = "GameObject " + std::to_string(id);
}

GameObject::GameObject(const std::string& name)
	: GameObject()
{
	if (!name.empty())
		this->name = name;
}

GameObject::~GameObject()
{
	for (const auto& component : components)
		delete component;
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::SetModel(const std::string& filePath)
{
	model = ResourceManager::Get<Model>(filePath);
}

void GameObject::AddChild(GameObject* child)
{
	if (child->parent == this)
		return;

	if (child->parent != nullptr)
		child->parent->RemoveChild(child);

	child->parent = this;
	children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	if (child->parent != this)
		return;

	child->parent = nullptr;
	children.remove(child);
}

void GameObject::UpdateRender() const
{
	if (model)
		model->Draw();
}

void GameObject::UpdateComponents() const
{
	for (const auto& component : components)
		component->Update();
}

void GameObject::UpdateHierarchy()
{
	if (this->parent != nullptr)
	{
		GetTransform().GetWorldRotation() = parent->GetTransform().GetOrientation() * GetTransform().GetLocalRotation();
		GetTransform().GetWorldPosition() = parent->GetTransform().GetWorldPosition() + (parent->GetTransform().GetOrientation() * GetTransform().GetLocalPosition());
	}

	else
	{
		GetTransform().GetWorldRotation() = GetTransform().GetLocalRotation();
		GetTransform().GetWorldPosition() = GetTransform().GetLocalPosition();
	}

	for (const auto& child : this->children)
		child->UpdateHierarchy();
}

void GameObject::AddComponent(Component* newComponent)
{
	components.push_back(newComponent);
}

Transform& GameObject::GetTransform()
{
	return transform;
}