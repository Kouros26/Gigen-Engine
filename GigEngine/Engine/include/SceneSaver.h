#pragma once
#include <string>
#include "Camera.h"
#include "Light.h"

namespace formatting
{
	const std::string skyboxTag("B");
	const std::string typeTag("*");
	const std::string lightValueTag(".");
	const std::string cameraValueTag("-");
	const std::string nameTag("N");
	const std::string transformTag("W");
	const std::string rigidBodyTag("R");
	const std::string parentTag("P");
	const std::string componentTag("C");
	const std::string modelTag("M");
	const std::string textureTag("T");
	const std::string newObjectTag("\n");
}

struct ProcessedObject // describes the gameObject that is currently handled
{
	inline static std::string type;
	inline static std::string name;
	inline static std::string otherValues;
	inline static std::string transform;
	inline static std::string rigidBody;
	inline static std::string parent;
	inline static std::string components;
	inline static std::string model;
	inline static std::string texture;

	inline static unsigned int componentSize;

	static void Clear();
};

class Scene
{
public:

	static void SaveScene(const std::string& pSceneName);
	static void LoadScene(const std::string& pSceneName);
	static void ReloadScene(const std::string& pSceneName);

private:

	static void GetValues(GameObject* pGameObject);
	static void GetLightValues(DirLight* pGameObject);
	static void GetCameraValues(const Camera* pGameObject);
	static bool IsLight(const std::string& pType);

	static GameObject* ProcessTypeOfGameObject(const std::string& pLine);
	static void ProcessLightData(const std::string& pLine, GameObject* pOutGameObject);
	static void ProcessCameraData(const std::string& pLine, Camera* pOutGameObject);
	static void ProcessTransform(const std::string& pLine, GameObject* pOutGameObject);
	static void ProcessRigidBody(const std::string& pLine, GameObject* pOutGameObject);
	static void ProcessComponents(const std::string& pLine, GameObject* pOutGameObject);

	static std::string VecToString(const float pFirst, const float pSecond, const float pThird);
	static std::string VecToString(const lm::FVec3& pVec);
	static std::vector<std::string> SplitString(const std::string& pString, char delimiter = ' ');

	inline static std::string sceneFolder = "Resources/Scenes/";

	inline static std::vector<std::pair<std::string, std::string>> parentChildMemory;
};
