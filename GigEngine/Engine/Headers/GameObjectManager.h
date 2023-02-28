#pragma once
#include <vector>
#include <string>

class GameObject;
class Camera;
class DirLight;
class SpotLight;
class PointLight;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	static unsigned int GetSize();
	static GameObject* GetGameObject(int i);
	static void Cleanup();
	static void AddGameObject(GameObject* object);
	static void Remove(GameObject* object);
	static std::vector<GameObject*> FindObjectsByName(std::string name);
	static Camera* GetCurrentCamera();
	static void SetCurrentCamera(Camera* camera);

	static void SendLightsToShader();
	static int GetDirLightSize();
	static int GetPointLightSize();
	static int GetSpotLightSize();

private:
	inline static Camera* currentCamera;
	inline static std::vector<GameObject*> gameObjects;

	inline static std::vector<DirLight*> dirLights;
	inline static std::vector<SpotLight*> spotLights;
	inline static std::vector<PointLight*> pointLights;
};
