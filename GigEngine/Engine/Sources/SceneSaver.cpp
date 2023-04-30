#include "SceneSaver.h"
#include <fstream>
#include <array>
#include "BoxRigidBody.h"
#include "CapsuleRigidBody.h"
#include "Component.h"
#include "GameObjectManager.h"
#include "Model.h"
#include "RigidBody.h"
#include "SphereRigidBody.h"
#include "Texture.h"
#include "Behaviour.h"

void Scene::SaveScene(const std::string& pSceneName)
{
	static std::ofstream file(sceneFolder + pSceneName);

	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		GetValues(GameObjectManager::GetGameObject(i));
		file << formatting::typeTag << ProcessedObject::type << std::endl;
		file << formatting::nameTag << ProcessedObject::name << std::endl;
		file << formatting::transformTag << ProcessedObject::transform << std::endl;
		file << formatting::rigidBodyTag << ProcessedObject::rigidBody << std::endl;
		file << formatting::parentTag << ProcessedObject::parent << std::endl;
		file << formatting::componentTag << std::to_string(ProcessedObject::componentSize) << ' ' << ProcessedObject::components << std::endl;
		file << formatting::modelTag << ProcessedObject::model << std::endl;
		file << formatting::textureTag << ProcessedObject::texture << std::endl;
		file << formatting::newObjectTag;
		ProcessedObject::components.clear();
	}

	file.close();
}

void Scene::LoadScene(const std::string& pSceneName)
{
	std::ifstream file(sceneFolder + pSceneName);
	std::string line;
	GameObject* obj = nullptr;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		const std::string& word = line.substr(1);

		switch (line[0])
		{
		case 'N':
			obj = GameObjectManager::CreateGameObject(word);
			break;

		case 'W':
			ProcessTransform(word, obj);
			break;

		case 'R':
			if (word != "none")
				ProcessRigidBody(word, obj);
			break;

		case 'P':
			if (word != "none")
				parentChildMemory.emplace_back(word, obj->GetName());
			break;

		case 'C':
			if (word[1] != '0')
				ProcessComponents(word, obj);
			break;

		case 'M':
			if (word != "none")
				obj->SetModel(word);
			break;

		case 'T':
			if (word != "none")
				obj->SetTexture(word);
			break;

		default:
			break;
		}
	}

	for (const auto& i : parentChildMemory)
		GameObjectManager::FindObjectByName(i.first)->AddChild(GameObjectManager::FindObjectByName(i.second));
}

void Scene::GetValues(GameObject* pGameObject)
{
	//type
	ProcessedObject::type = pGameObject->GetType();

	//name
	ProcessedObject::name = pGameObject->GetName();

	//transform
	const lm::FVec3& position = pGameObject->GetTransform().GetWorldPosition();
	const lm::FVec3& rotation = pGameObject->GetTransform().GetWorldRotation();
	const lm::FVec3& scale = pGameObject->GetTransform().GetWorldScale();
	ProcessedObject::transform = VecToString(position) + ' ' + VecToString(rotation) + ' ' + VecToString(scale);

	//rigidbody
	if (!pGameObject->GetRigidBody())
		ProcessedObject::rigidBody = "none";

	else
	{
		if (pGameObject->GetRigidBody()->GetShapeType() == RigidBodyType::BOX)
		{
			const auto boxBody = static_cast<BoxRigidBody*>(pGameObject->GetRigidBody());
			const lm::FVec3& halfExtents = boxBody->GetHalfExtents();
			ProcessedObject::rigidBody = "box " + VecToString(halfExtents);
		}

		else if (pGameObject->GetRigidBody()->GetShapeType() == RigidBodyType::CAPSULE)
		{
			const auto capsuleBody = static_cast<CapsuleRigidBody*>(pGameObject->GetRigidBody());
			const float capsuleRadius = capsuleBody->GetRadius();
			const float height = capsuleBody->GetHeight();
			ProcessedObject::rigidBody = "capsule " + std::to_string(capsuleRadius) + ' ' + std::to_string(height);
		}

		else if (pGameObject->GetRigidBody()->GetShapeType() == RigidBodyType::SPHERE)
		{
			const auto sphereBody = static_cast<SphereRigidBody*>(pGameObject->GetRigidBody());
			const float sphereRadius = sphereBody->GetRadius();
			ProcessedObject::rigidBody = "sphere " + std::to_string(sphereRadius);
		}

		const lm::FVec3& rbScale = pGameObject->GetRigidBody()->GetScale();
		const float mass = pGameObject->GetRigidBody()->GetMass();
		ProcessedObject::rigidBody += ' ' + VecToString(rbScale) + ' ' + std::to_string(mass);
		pGameObject->GetRigidBody()->IsGravityEnabled() ? ProcessedObject::rigidBody += " true" : ProcessedObject::rigidBody += " false";
	}

	//parent
	!pGameObject->GetParent() ? ProcessedObject::parent = "none" : ProcessedObject::parent = pGameObject->GetParent()->GetName();

	//components
	for (ProcessedObject::componentSize = 0; ProcessedObject::componentSize < pGameObject->GetComponentCount(); ProcessedObject::componentSize++)
	{
		const std::string& component = dynamic_cast<GigScripting::Behaviour*>(pGameObject->GetComponentByID(ProcessedObject::componentSize))->GetScriptName();
		ProcessedObject::components += component + ' ';
	}

	//model
	!pGameObject->GetModel() ? ProcessedObject::model = "none" : ProcessedObject::model = pGameObject->GetModel()->GetFilePath();

	//texture
	!pGameObject->GetTexture() ? ProcessedObject::texture = "none" : ProcessedObject::texture = pGameObject->GetTexture()->GetFilePath();
}

void Scene::ProcessTransform(const std::string& pLine, GameObject* pGameObject)
{
	std::array<float, 9> data{};
	unsigned int dataIncr = 0;
	const std::vector<std::string> strings = SplitString(pLine);

	for (int i = 0; i < strings.size(); i++)
		data.at(i) = std::stof(strings[i]);

	pGameObject->GetTransform() = Transform(lm::FVec3{ data[0], data[1], data[2] },
											lm::FVec3{ data[3], data[4], data[5] },
											lm::FVec3{ data[6], data[7], data[8] });
}
//
void Scene::ProcessRigidBody(const std::string& pLine, GameObject* pGameObject)
{
	const std::vector<std::string> strings = SplitString(pLine);

	if (strings[0] == "box")
	{
		pGameObject->CreateBoxRigidBody({ std::stof(strings[1]), std::stof(strings[2]), std::stof(strings[3]) },
													{ std::stof(strings[4]), std::stof(strings[5]), std::stof(strings[6]) },
															std::stof(strings[7]));

		if (strings[8] == "false")
			pGameObject->GetRigidBody()->SetGravityEnabled(false);
	}

	else if (strings[0] == "capsule")
	{
		pGameObject->CreateCapsuleRigidBody(std::stof(strings[1]), std::stof(strings[2]),
										{ std::stof(strings[3]), std::stof(strings[4]), std::stof(strings[5]) },
												std::stof(strings[6]));

		if (strings[7] == "false")
			pGameObject->GetRigidBody()->SetGravityEnabled(false);
	}

	else if (strings[0] == "sphere")
	{
		pGameObject->CreateSphereRigidBody(std::stof(strings[1]), { std::stof(strings[2]),
			std::stof(strings[3]), std::stof(strings[4]) }, std::stof(strings[5]));

		if (strings[6] == "false")
			pGameObject->GetRigidBody()->SetGravityEnabled(false);
	}
}

void Scene::ProcessComponents(const std::string& pLine, GameObject* pGameObject)
{
	const std::vector<std::string> strings = SplitString(pLine);

	for (int i = 1; i < strings.size(); i++)
	{
		pGameObject->AddComponent<GigScripting::Behaviour>(strings[i]);
	}
}

std::string Scene::VecToString(const lm::FVec3& pVec)
{
	return std::to_string(pVec.x) + ' ' + std::to_string(pVec.y) + ' ' + std::to_string(pVec.z);
}

std::vector<std::string> Scene::SplitString(const std::string& pString, char delimiter)
{
	int start = 0;
	int end = pString.find(delimiter);
	std::vector<std::string> strings;
	while (end != -1) 
	{
		strings.push_back(pString.substr(start, end - start));
		start = end + 1;
		end = pString.find(delimiter, start);
	}
	strings.push_back(pString.substr(start, end - start));

	return strings;
}