#pragma once
#include <string>
#include "GameObject.h"

namespace formatting
{
	const std::string typeTag("*");
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
	inline static std::string transform;
	inline static std::string rigidBody;
	inline static std::string parent;
	inline static std::string components;
	inline static std::string model;
	inline static std::string texture;

	inline static unsigned int componentSize;
	inline static unsigned int scriptSize;
};

class Scene
{
public:

	static void SaveScene(const std::string& pSceneName);
	static void LoadScene(const std::string& pSceneName);

private:

	static void GetValues(GameObject* pGameObject);
	static void ProcessTransform(const std::string& pLine, GameObject* pGameObject);
	static void ProcessRigidBody(const std::string& pLine, GameObject* pGameObject);
	static void ProcessComponents(const std::string& pLine, GameObject* pGameObject);

	static std::string VecToString(const lm::FVec3& pVec);
	static std::vector<std::string> SplitString(const std::string& pString, char delimiter = ' ');

	inline static std::string sceneFolder = "Resources/Scenes/";

	inline static std::vector<std::pair<std::string, std::string>> parentChildMemory;
};
