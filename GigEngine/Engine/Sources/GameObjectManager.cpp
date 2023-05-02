#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "RigidBody.h"
#include <algorithm>

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{
	Cleanup();
}

unsigned int GameObjectManager::GetSize()
{
	return static_cast<unsigned>(gameObjects.size());
}

GameObject*& GameObjectManager::GetGameObject(int i)
{
	return gameObjects[i];
}

void GameObjectManager::Cleanup()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
		delete gameObjects[i];

	delete skybox;

	gameObjects.clear();
	dirLights.clear();
	pointLights.clear();
	spotLights.clear();
}

GameObject* GameObjectManager::CreateGameObject()
{
	const auto object = new GameObject();

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateGameObject(const std::string& name)
{
	const auto object = new GameObject(name);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateGameObject(const std::string& name, const lm::FVec3& position,
	const lm::FVec3& rotation, const lm::FVec3& scale)
{
	const auto object = new GameObject(name, position, rotation, scale);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateGameObject(const lm::FVec3& position, const lm::FVec3& rotation,
	const lm::FVec3& scale)
{
	const auto object = new GameObject(position, rotation, scale);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateGameObject(const GameObject& other)
{
	const auto object = new GameObject(other);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateGameObject(const GameObject* other)
{
	const auto object = new GameObject(*other);

	return AddGameObject(object);
}

GameObject& GameObjectManager::operator=(const GameObject& other) const
{
	const auto object = new GameObject(other);

	return *AddGameObject(object);
}

GameObject* GameObjectManager::CreateSpotLight(float ambient, float diffuse, float specular,
	float constant, float linear, float quadratic,
	float cutOff, float outerCutOff,
	const lm::FVec3& color)
{
	if (spotLights.size() >= g_nbMaxLight)
		return nullptr;

	const auto object = new SpotLight(ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, color);
	spotLights.push_back(object);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreatePointLight(float ambient, float diffuse, float specular,
	float constant, float linear, float quadratic,
	const lm::FVec3& color)
{
	if (pointLights.size() >= g_nbMaxLight)
		return nullptr;

	const auto object = new PointLight(ambient, diffuse, specular, constant, linear, quadratic, color);
	pointLights.push_back(object);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateDirLight(float ambient, float diffuse, float specular,
                                              const lm::FVec3& color)
{
	if (dirLights.size() >= g_nbMaxLight)
		return nullptr;

	const auto object = new DirLight(ambient, diffuse, specular, color);
	dirLights.push_back(object);

	return AddGameObject(object);
}

GameObject* GameObjectManager::CreateCamera()
{
	const auto object = new Camera();

	if (!currentCamera)
		currentCamera = object;

	return AddGameObject(object);
}

void GameObjectManager::RemoveGameObject(GameObject* object)
{
	const auto it = std::ranges::find(gameObjects, object);

	if (it == gameObjects.end())
		return;

	gameObjects.erase(it);

	const auto pointsTemp = std::ranges::find(pointLights, object);

	if (pointsTemp != pointLights.end())
	{
		pointLights.erase(pointsTemp);
		object->~GameObject();
		return;
	}

	const auto spotTemp = std::ranges::find(spotLights, object);

	if (spotTemp != spotLights.end())
	{
		spotLights.erase(spotTemp);
		object->~GameObject();
		return;
	}

	const auto dirTemp = std::ranges::find(dirLights, object);

	if (dirTemp != dirLights.end())
	{
		dirLights.erase(dirTemp);
		object->~GameObject();
		return;
	}

	object->~GameObject();
}

std::vector<GameObject*> GameObjectManager::FindObjectsByName(const std::string& name)
{
	std::vector<GameObject*> namedObjects;

	for (auto& gameObject : gameObjects)
		if (gameObject->GetName() == name)
			namedObjects.push_back(gameObject);

	return namedObjects;
}

GameObject* GameObjectManager::FindObjectByName(const std::string& name)
{
	for (const auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == name)
			return gameObject;
	}
	return nullptr;
}

GameObject* GameObjectManager::FindObjectById(unsigned int id)
{
	for (const auto& gameObject : gameObjects)
	{
		if (gameObject->GetId() == id)
		{
			return gameObject;
		}
	}
	return nullptr;
}

Camera* GameObjectManager::GetCurrentCamera()
{
	return currentCamera;
}

void GameObjectManager::SetCurrentCamera(Camera* camera)
{
	currentCamera = camera;
}

void GameObjectManager::CreateSkyBox()
{
	delete skybox;
	skybox = new Skybox();
}

Skybox*& GameObjectManager::GetSkyBox()
{
	return skybox;
}

void GameObjectManager::SendLightsToShader()
{
	for (int i = 0; i < dirLights.size(); i++)
	{
		dirLights[i]->SendToShader(i, g_dirLightShaderName);
	}
	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->SendToShader(i, g_pointLightShaderName);
	}
	for (int i = 0; i < spotLights.size(); i++)
	{
		spotLights[i]->SendToShader(i, g_spotLightShaderName);
	}
}

int GameObjectManager::GetDirLightSize()
{
	int nbLight = 0;
	for (DirLight* l : dirLights)
	{
		if (l->IsActive())
		{
			nbLight++;
		}
	}
	return nbLight;
}

int GameObjectManager::GetPointLightSize()
{
	int nbLight = 0;
	for (PointLight* l : pointLights)
	{
		if (l->IsActive())
		{
			nbLight++;
		}
	}
	return nbLight;
}

int GameObjectManager::GetSpotLightSize()
{
	int nbLight = 0;
	for (SpotLight* l : spotLights)
	{
		if (l->IsActive())
		{
			nbLight++;
		}
	}
	return nbLight;
}

void GameObjectManager::SetFocusedGameObject(GameObject* obj)
{
	focusedObject = obj;
}

GameObject* GameObjectManager::GetFocusedGameObject()
{
	return focusedObject;
}

GameObject* GameObjectManager::AddGameObject(GameObject* object)
{
	gameObjects.push_back(object);
	SetFocusedGameObject(object);
	return object;
}