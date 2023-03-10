#include "GameObjectManager.h"
//#include "Model.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
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
	for (const auto& gameObject : gameObjects)
		delete gameObject;

	//if (SkyBox)
	//	delete SkyBox;
}

void GameObjectManager::AddGameObject(GameObject* object)
{
	if (std::find(gameObjects.begin(), gameObjects.end(), object) == gameObjects.end())
		gameObjects.push_back(object);
}

std::vector<GameObject*> GameObjectManager::FindObjectsByName(std::string name)
{
	std::vector<GameObject*> namedObjects;

	for (auto& gameObject : gameObjects)
		if (gameObject->GetName() == name)
			namedObjects.push_back(gameObject);

	return namedObjects;
}

Camera* GameObjectManager::GetCurrentCamera()
{
	return currentCamera;
}

void GameObjectManager::SetCurrentCamera(Camera* camera)
{
	currentCamera = camera;
}

//void GameObjectManager::InitSkyBox(Model* model)
//{
//	SkyBox = model;
//}
//
//Model* GameObjectManager::GetSkyBox()
//{
//	return SkyBox;
//}