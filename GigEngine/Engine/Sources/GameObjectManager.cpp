#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"

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
    for (int i = 0; i < GameObjectManager::GetSize(); i++)
        delete gameObjects[i];
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

GameObject& GameObjectManager::operator=(const GameObject& other)
{
    const auto object = new GameObject(other);

    return *AddGameObject(object);
}

GameObject* GameObjectManager::CreateSpotLight(float ambient, float diffuse, float specular,
    float constant, float linear, float quadratic,
    float cutOff, float outerCutOff,
    lm::FVec3 color)
{
    if (spotLights.size() >= g_nbMaxLight)
        return nullptr;

    SpotLight* object = new SpotLight(ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, color);
    spotLights.push_back(object);

    return AddGameObject(object);
}

GameObject* GameObjectManager::CreatePointLight(float ambient, float diffuse, float specular,
    float constant, float linear, float quadratic,
    lm::FVec3 color)
{
    if (pointLights.size() >= g_nbMaxLight)
        return nullptr;

    PointLight* object = new PointLight(ambient, diffuse, specular, constant, linear, quadratic, color);
    pointLights.push_back(object);

    return AddGameObject(object);
}

GameObject* GameObjectManager::CreateDirLight(float ambient, float diffuse, float specular,
    lm::FVec3 color)
{
    if (dirLights.size() >= g_nbMaxLight)
        return nullptr;

    DirLight* object = new DirLight(ambient, diffuse, specular, color);
    dirLights.push_back(object);

    return AddGameObject(object);
}

GameObject* GameObjectManager::CreateCamera()
{
    Camera* object = new Camera();

    return AddGameObject(object);
}

void GameObjectManager::Remove(GameObject* object)
{
    auto it = std::find(gameObjects.begin(), gameObjects.end(), object);

    if (it != gameObjects.end())
        gameObjects.erase(it);

    PointLight* point = dynamic_cast<PointLight*>(object);
    if (point)
    {
        auto temp = std::find(pointLights.begin(), pointLights.end(), point);

        if (temp != pointLights.end())
            pointLights.erase(temp);
        return;
    }

    SpotLight* spot = dynamic_cast<SpotLight*>(object);
    if (spot)
    {
        auto temp = std::find(spotLights.begin(), spotLights.end(), spot);

        if (temp != spotLights.end())
            spotLights.erase(temp);
        return;
    }

    DirLight* dir = dynamic_cast<DirLight*>(object);
    if (dir)
    {
        auto temp = std::find(dirLights.begin(), dirLights.end(), dir);

        if (temp != dirLights.end())
            dirLights.erase(temp);
    }
}

std::vector<GameObject*> GameObjectManager::FindObjectsByName(std::string name)
{
    std::vector<GameObject*> namedObjects;

    for (auto& gameObject : gameObjects)
        if (gameObject->GetName() == name)
            namedObjects.push_back(gameObject);

    return namedObjects;
}

GameObject* GameObjectManager::FindObjectByName(std::string name)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->GetName() == name)
        {
            return gameObjects[i];
        }
    }
    return nullptr;
}

GameObject* GameObjectManager::FindObjectById(int id)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->GetId() == id)
        {
            return gameObjects[i];
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
    return dirLights.size();
}

int GameObjectManager::GetPointLightSize()
{
    return pointLights.size();
}

int GameObjectManager::GetSpotLightSize()
{
    return spotLights.size();
}

GameObject* GameObjectManager::AddGameObject(GameObject* object)
{
    gameObjects.push_back(object);
    return object;
}