#include "GameObject.h"
#include "Model.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "Component.h"

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

GameObject::GameObject(const GameObject& other)
{
	gameObjectIndex++;
	id = gameObjectIndex;

	name = other.name + " " + std::to_string(id);

	transform = other.transform;

	for (const auto& component : other.components)
		components.push_back(component->Clone(this));

	for (const auto& script : other.scripts)
	{
		auto newScript = dynamic_cast<Script*>(script->Clone(this));
		scripts.push_back(newScript);
		newScript->Awake();
	}

	for (const auto& child : other.children)
		AddChild(GameObjectManager::CreateGameObject(*child));

	if (other.model != nullptr)
		model = new Model(*other.model);
}

GameObject& GameObject::operator=(const GameObject& other)
{
	if (this == &other)
		return *this;

	name = other.name + " " + std::to_string(id);

	transform = other.transform;

	for (const auto& component : other.components)
		components.push_back(component->Clone(this));

	for (const auto& child : other.children)
		AddChild(GameObjectManager::CreateGameObject(*child));

	if (other.model != nullptr)
		model = new Model(*other.model);

	return *this;
}

GameObject::~GameObject()
{
	for (const auto& component : components)
		delete component;

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

void GameObject::SetName(const std::string& pName)
{
	if (pName.length() == 0)
	{
		name = "GameObject " + std::to_string(id);
		return;
	}

	name = pName;
}

unsigned int GameObject::GetId()
{
	return id;
}

void GameObject::SetModel(std::string const& filePath)
{
	model = ResourceManager::Get<Model>(filePath);
	if (!texture) 
	{
		texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
	}
}

Model* GameObject::GetModel()
{
	return model;
}

void GameObject::SetTexture(const std::string& filePath)
{
	texture = ResourceManager::Get<Texture>(filePath);
	if (!texture->isValid())
	{
		std::cout << "texture invalid" << std::endl;
		texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
	}
}

Texture* GameObject::GetTexture()
{
	return texture;
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
		model->Draw(texture);
}

void GameObject::UpdateComponents() const
{
	for (const auto& component : components)
		component->Update();
}

void GameObject::LateUpdate() const
{
	for (const auto& script : scripts)
		script->LateUpdate();
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

void GameObject::AddComponent(Component* newComponent)
{
	components.push_back(newComponent);

	if (const auto script = dynamic_cast<Script*>(newComponent))
	{
		scripts.push_back(script);
		script->Awake();
	}
}

unsigned GameObject::GetComponentCount() const
{
	return static_cast<unsigned>(components.size());
}

Transform& GameObject::GetTransform()
{
	return transform;
}