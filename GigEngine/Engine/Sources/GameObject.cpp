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

GameObject::GameObject(const std::string& name, const lm::FVec3& position, const lm::FVec3& rotation,
	const lm::FVec3& scale)
		: transform(position, rotation, scale)
{
	gameObjectIndex++;
	id = gameObjectIndex;

	if (!name.empty())
		this->name = name;

	else
		this->name = "GameObject " + std::to_string(id);
}

GameObject::GameObject(const lm::FVec3& position, const lm::FVec3& rotation, const lm::FVec3& scale)
	: transform(position, rotation, scale)
{
	gameObjectIndex++;
	id = gameObjectIndex;

	name = "GameObject " + std::to_string(id);
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

	child->GetTransform().AssignLocalPosition(child->GetTransform().GetWorldPosition() - GetTransform().GetWorldPosition());
	child->GetTransform().AssignLocalScale(child->GetTransform().GetWorldScale() / GetTransform().GetWorldScale());

	children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	if (child->parent != this)
		return;

	child->GetTransform().SetLocalPosition(child->GetTransform().GetWorldPosition());
	child->GetTransform().SetLocalRotation(child->GetTransform().GetWorldRotation());

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
		GetTransform().AssignWorldRotation(parent->GetTransform().GetWorldRotation() + GetTransform().GetLocalRotation());
		GetTransform().AssignWorldPosition(parent->GetTransform().GetWorldPosition() + (parent->GetTransform().GetOrientation() * GetTransform().GetLocalPosition()));
		GetTransform().AssignWorldScale(parent->GetTransform().GetWorldScale() * GetTransform().GetLocalScale());
	}

	else
	{
		GetTransform().AssignWorldRotation(GetTransform().GetLocalRotation());
		GetTransform().AssignWorldPosition(GetTransform().GetLocalPosition());
		GetTransform().AssignWorldScale(GetTransform().GetLocalScale());
	}

	for (const auto& child : this->children)
		child->UpdateHierarchy();
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