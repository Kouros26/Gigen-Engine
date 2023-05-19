#pragma once
#include "IResource.h"
#include "ModelLoader.h"
#include <vector>
#include <map>

const char* const g_defaultModelPath = "Engine/Models/Basics/Cube.FBX";

class Texture;
class Material;
class Mesh;

class Model : public IResource, public ModelLoader
{
public:
	Model(std::string const& pFilePath);
	~Model();

	Model(const Model& other);
	Model(Model&& other) noexcept;
	Model& operator=(const Model& other);
	Model& operator=(Model&& other) noexcept;
	void Draw(Texture* texture) const;

	void Init() override;

	auto& GetBoneInfoMap();
	int& GetBoneCount();

private:

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	std::map<std::string, BoneInfo> boneMap;
	int boneCounter = 0;
};