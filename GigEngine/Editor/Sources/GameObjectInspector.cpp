#include <Windows.h>
#include "GameObjectInspector.h"
#include "InterfaceManager.h"
#include "ToolsDisplay.h"
#include "imgui.h"
#include "Application.h"
#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "CapsuleRigidBody.h"
#include "SphereRigidBody.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include <Windows.h>
#include <filesystem>
#include "Behaviour.h"

GameObjectInspector::GameObjectInspector()
{
	InterfaceManager::AddEditorElement(this);
}

GameObjectInspector::~GameObjectInspector()
= default;

void GameObjectInspector::Draw()
{
	height = InterfaceManager::GetHeight() - InterfaceManager::GetClassHeight<ToolsDisplay>() - g_menuBarSize;
	ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width, g_menuBarSize });
	ImGui::SetNextWindowSize({ width, height });

	// noMove et NoCollapse
	ImGui::Begin("Inspector", NULL, 4 | 32);

	LimitWidthResize();
	ImGui::SetWindowSize("Inspector", { width, height });

	DrawGameObject();

	ImGui::End();
}

void GameObjectInspector::DrawGameObject()
{
	GameObject* object = GameObjectManager::GetFocusedGameObject();
	if (!object) return;

	static char name[128];
	strcpy_s(name, object->GetName().c_str());

	ImGui::Text(ICON_GAMEOBJECT " Name"); ImGui::SameLine();

	if (ImGui::InputText("##1", name, 128))
	{
		object->SetName(name);
	}

	bool isActive = object->IsActive();
	ImGui::SameLine();
	ImGui::Checkbox("##2", &isActive);
	if (isActive != object->IsActive())
	{
		object->SetActive(isActive);
	}

	ImGui::Separator();

	DrawTransform(object);
	DrawSpecials(object);

	if (object->GetRigidBody())
		DrawRigidBody(object);

	if (object->GetModel())
		DrawModel(object);

	DrawComponents(object);
	ImGui::Separator();
	DrawAddComponent(object);

	DrawDropTarget(object);
}

void GameObjectInspector::DrawTransform(GameObject * pObject) const
{
	if (ImGui::CollapsingHeader(ICON_TRANSFORM " Transform"))
	{
		const lm::FVec3 rot = pObject->GetTransform().GetWorldRotation();
		const lm::FVec3 pos = pObject->GetTransform().GetWorldPosition();
		const lm::FVec3 scl = pObject->GetTransform().GetWorldScale();

		float translation[] = { pos.x, pos.y, pos.z };
		float scale[] = { scl.x, scl.y, scl.z };
		float rotation[] = { rot.x, rot.y, rot.z };

		ImGui::Text("Position"); ImGui::SameLine();

		if (ImGui::DragFloat3("##3", translation, g_maxStep, g_floatMin, g_floatMax, g_floatFormat))
		{
			pObject->GetTransform().SetWorldPosition(lm::FVec3(translation[0], translation[1], translation[2]));
		}

		ImGui::Text("Scale"); ImGui::SameLine();
		if (ImGui::DragFloat3("##4", scale, g_maxStep, 0.000000001f, g_floatMax, g_floatFormat))
		{
			pObject->GetTransform().SetWorldScale(lm::FVec3(scale[0], scale[1], scale[2]));
		}

		ImGui::Text("Rotation"); ImGui::SameLine();
		if (ImGui::DragFloat3("##5", rotation, g_maxStep, -360.0f, 360.0f, g_floatFormat))
		{
			pObject->GetTransform().SetWorldRotation(lm::FVec3(rotation[0], rotation[1], rotation[2]));
		}
	}
}

void GameObjectInspector::DrawModel(GameObject * pObject) const
{
	if (ImGui::CollapsingHeader(ICON_MODEL " Model"))
	{
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("ModelPopUp");
		}

		if (ImGui::BeginPopup("ModelPopUp"))
		{
			ImGui::SeparatorText("Model");
			if (ImGui::MenuItem("Remove"))
			{
				pObject->SetModel(nullptr);
			}
			ImGui::EndPopup();
			if (!pObject->GetModel())
			{
				return;
			}
		}

		std::string path = pObject->GetModel()->GetFilePath();

		ImGui::Text("Path :"); ImGui::SameLine();
		ImGui::Text(path.c_str());

		if (ImGui::Button("Locate##1"))
		{
			const std::string& filePath = GetFilePathFromExplorer("3D object \0 *.obj; *.OBJ; *.fbx; *.FBX\0");

			if (filePath.length() > 0)
				pObject->SetModel(filePath);
		}

		if (pObject->GetModel())
		{
			DrawTexture(pObject);
		}
	}
}

void GameObjectInspector::DrawTexture(GameObject * pObject) const
{
	ImGui::SetCursorPosX(30);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader(ICON_TEXTURE " Texture"))
	{
		std::string path;
		if (pObject->GetTexture())
		{
			path = pObject->GetTexture()->GetFilePath();
		}

		ImGui::Text("Path :"); ImGui::SameLine();
		ImGui::Text(path.c_str());

		if (ImGui::Button("Locate##2"))
		{
			const std::string& filePath = GetFilePathFromExplorer("image \0 *.png; *.jpeg; *.jpg\0");

			if (filePath.length() > 0)
				pObject->SetTexture(filePath);
		}
	}
	ImGui::EndGroup();
}

void GameObjectInspector::DrawRigidBody(GameObject * pObject) const
{
	if (ImGui::CollapsingHeader("RigidBody"))
	{
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("RigidPopUp");
		}

		if (ImGui::BeginPopup("RigidPopUp"))
		{
			ImGui::SeparatorText("RigidBody");
			if (ImGui::MenuItem("Remove"))
			{
				pObject->RemoveRigidBody();
			}
			ImGui::EndPopup();
			if (!pObject->GetRigidBody())
			{
				return;
			}
		}

		RigidBody* rigid = pObject->GetRigidBody();

		ImGui::Text("Mass"); ImGui::SameLine();
		btScalar mass = rigid->GetMass();
		if (ImGui::DragFloat("##18", &mass, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			rigid->SetMass(mass);
		}

		ImGui::Text("Gravity"); ImGui::SameLine();
		bool grav = rigid->IsGravityEnabled();
		ImGui::Checkbox("##19", &grav);
		if (grav != rigid->IsGravityEnabled())
		{
			rigid->SetGravityEnabled(grav);
		}

		ImGui::Text("Collision type"); ImGui::SameLine();
		const char* items[] = { "Dynamic", "Kinetic", "Static" };
		int item_current = rigid->GetCollisionFlag();
		ImGui::Combo("##20", &item_current, items, IM_ARRAYSIZE(items));
		if (item_current != rigid->GetCollisionFlag())
		{
			rigid->SetRBState(static_cast<RBState>(item_current));
		}

		DrawRigidShape(rigid);
	}
}

void GameObjectInspector::DrawRigidShape(RigidBody * body) const
{
	ImGui::SetCursorPosX(30);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Shape"))
	{
		if (body->GetShapeType() == RigidBodyType::CAPSULE)
		{
			const auto caps = static_cast<CapsuleRigidBody*>(body);
			float radius = caps->GetRadius();
			float height = caps->GetHeight();

			ImGui::Text("Radius"); ImGui::SameLine();
			if (ImGui::DragFloat("##21", &radius, g_maxStep, 0.001f, g_floatMax, g_floatFormat))
			{
				caps->SetRadius(radius);
			}
			ImGui::Text("Height"); ImGui::SameLine();
			if (ImGui::DragFloat("##22", &height, g_maxStep, 0.001f, g_floatMax, g_floatFormat))
			{
				caps->SetHeight(height);
			}
		}
		else if (body->GetShapeType() == RigidBodyType::SPHERE)
		{
			const auto sphere = static_cast<SphereRigidBody*>(body);
			float radius = sphere->GetRadius();

			ImGui::Text("Radius"); ImGui::SameLine();
			if (ImGui::DragFloat("##23", &radius, g_maxStep, 0.001f, g_floatMax, g_floatFormat))
			{
				sphere->SetRadius(radius);
			}
		}
	}

	if (ImGui::CollapsingHeader("Transform##2"))
	{
		//float rotation[3];
		//body->GetTransfrom().getRotation().getEulerZYX((btScalar&)rotation[2], (btScalar&)rotation[1], (btScalar&)rotation[0]);

		//btVector3 btPos = body->GetTransfrom().getOrigin();
		//float translation[3] = { btPos.getX(), btPos.getY(), btPos.getZ() };

		float scale[3] = { body->GetScale().x, body->GetScale().y, body->GetScale().z };

		//ImGui::Text("Position"); ImGui::SameLine();
		//if (ImGui::DragFloat3("##21", translation, g_maxStep, g_floatMin, g_floatMax, g_floatFormat))
		//{
		//	body->SetRigidBodyPosition(lm::FVec3(translation[0], translation[1], translation[2]));
		//}

		ImGui::Text("Scale"); ImGui::SameLine();
		if (ImGui::DragFloat3("##22", scale, g_maxStep, 0.001f, g_floatMax, g_floatFormat))
		{
			body->SetRigidBodyScale(lm::FVec3(scale[0], scale[1], scale[2]));
		}

		//ImGui::Text("Rotation"); ImGui::SameLine();
		//if (ImGui::DragFloat3("##23", rotation, g_maxStep, -360.0f, 360.0f, g_floatFormat))
		//{
		//	body->SetRigidBodyRotation(lm::FVec3(rotation[0], rotation[1], rotation[2]));
		//}
	}
	ImGui::EndGroup();
}

void GameObjectInspector::DrawSpecials(GameObject * pObject) const
{
	if (auto light = dynamic_cast<DirLight*>(pObject))
	{
		DrawLight(pObject);
		return;
	}

	if (const auto cam = dynamic_cast<Camera*>(pObject))
		DrawCamera(cam);
}

void GameObjectInspector::DrawComponents(GameObject * pObject) const
{
	std::vector<GigScripting::Behaviour*> scripts;
	pObject->GetComponents<GigScripting::Behaviour>(scripts);
	if (scripts.size() == 0)
	{
		return;
	}
	using namespace GigScripting;
	if (ImGui::CollapsingHeader(ICON_COMPONENT " Scripts"))
	{
		for (auto& script : scripts)
		{
			if (!script)
			{
				return;
			}
			const std::string& name = script->GetScriptName();
			if (ImGui::TreeNode(name.c_str()))
			{
				if (ImGui::IsItemClicked(1))
				{
					ImGui::OpenPopup("ScriptPopUp");
				}

				if (ImGui::BeginPopup("ScriptPopUp"))
				{
					ImGui::SeparatorText("Script");
					if (ImGui::MenuItem("Remove"))
					{
						pObject->RemoveScript(script);
					}
					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}

	scripts.clear();
}

void GameObjectInspector::DrawLight(GameObject * pObject) const
{
	const auto dirlight = dynamic_cast<DirLight*>(pObject);
	if (ImGui::CollapsingHeader(ICON_LIGHT " Light"))
	{
		float* color = dirlight->GetColor();
		float ambient = dirlight->GetAmbient();
		float diffuse = dirlight->GetDiffuse();
		float specular = dirlight->GetSpecular();

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##6", color);

		ImGui::Text("Ambient"); ImGui::SameLine();
		if (ImGui::DragFloat("##7", &ambient, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetAmbient(ambient);
		}

		ImGui::Text("Diffuse"); ImGui::SameLine();
		if (ImGui::DragFloat("##8", &diffuse, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetDiffuse(diffuse);
		}

		ImGui::Text("Specular"); ImGui::SameLine();
		if (ImGui::DragFloat("##9", &specular, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetSpecular(specular);
		}

		if (const auto pointLight = dynamic_cast<PointLight*>(pObject))
		{
			float constant = pointLight->GetConstant();
			float linear = pointLight->GetLinear();
			float quadratic = pointLight->GetQuadratic();

			ImGui::Text("Constant"); ImGui::SameLine();
			if (ImGui::DragFloat("##10", &constant, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetConstant(constant);
			}

			ImGui::Text("Linear"); ImGui::SameLine();
			if (ImGui::DragFloat("##11", &linear, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetLinear(linear);
			}

			ImGui::Text("Quadratic"); ImGui::SameLine();
			if (ImGui::DragFloat("##12", &quadratic, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetQuadratic(quadratic);
			}
		}

		if (const auto spotLight = dynamic_cast<SpotLight*>(pObject))
		{
			float cutOff = spotLight->GetCutOff();
			float outerCutOff = spotLight->GetOuterCutOff();

			ImGui::Text("CutOff"); ImGui::SameLine();
			if (ImGui::DragFloat("##13", &cutOff, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				spotLight->SetCutOff(cutOff);
			}

			ImGui::Text("OuterCutOff"); ImGui::SameLine();
			if (ImGui::DragFloat("##14", &outerCutOff, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				spotLight->SetOuterCutOff(outerCutOff);
			}
		}
	}
}

void GameObjectInspector::DrawCamera(Camera * pObject) const
{
	float fov = pObject->GetFov();
	float tNear = pObject->GetNear();
	float tFar = pObject->GetFar();

	if (ImGui::CollapsingHeader(ICON_CAMERA " Camera"))
	{
		ImGui::Text("Fov"); ImGui::SameLine();
		if (ImGui::DragFloat("##15", &fov, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetFov(fov);
		}

		ImGui::Text("Near"); ImGui::SameLine();
		if (ImGui::DragFloat("##16", &tNear, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetNear(tNear);
		}

		ImGui::Text("Far"); ImGui::SameLine();
		if (ImGui::DragFloat("##17", &tFar, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetFar(tFar);
		}
	}
}

void GameObjectInspector::DrawAddComponent(GameObject * pObject) const
{
	const ImGuiStyle& style = ImGui::GetStyle();

	const float size = ImGui::CalcTextSize("Add component " ICON_PLUS).x + style.FramePadding.x * 2.0f;
	const float avail = ImGui::GetContentRegionAvail().x;

	const float off = (avail - size) * 0.5f;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	if (ImGui::Button("Add component " ICON_PLUS))
	{
		ImGui::OpenPopup("addComponentPopUp");
	}

	if (ImGui::BeginPopup("addComponentPopUp"))
	{
		ImGui::SeparatorText("Components");
		if (!pObject->GetModel())
		{
			if (ImGui::MenuItem(ICON_MODEL " Model"))
			{
				pObject->SetModel(g_defaultModelPath);
			}
		}

		if (!pObject->GetRigidBody())
		{
			if (ImGui::MenuItem("RigidBody Capsule"))
			{
				pObject->CreateCapsuleRigidBody(1, 2, { 1 }, 1);
			}
			if (ImGui::MenuItem("RigidBody Cube"))
			{
				pObject->CreateBoxRigidBody({ 1 }, { 1 }, 1);
			}
			if (ImGui::MenuItem("RigidBody Sphere"))
			{
				pObject->CreateSphereRigidBody(1, { 1 }, 1);
			}
		}

		if (ImGui::MenuItem(ICON_COMPONENT " Scripts"))
		{
			pObject->AddScript();
		}

		ImGui::EndPopup();
	}
}

void GameObjectInspector::DrawDropTarget(GameObject * pObject) const
{
	ImGui::BeginChild("##");
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const char* path = static_cast<const char*>(payload->Data);
			const std::string str(path);
			if (str.find(".obj") != std::string::npos
				|| str.find(".OBJ") != std::string::npos
				|| str.find(".fbx") != std::string::npos
				|| str.find(".FBX") != std::string::npos)
			{
				pObject->SetModel(path);
			}
			else if (str.find(".png") != std::string::npos ||
				str.find(".jpg") != std::string::npos ||
				str.find(".jpeg") != std::string::npos)
			{
				pObject->SetTexture(path);
			}
			else if (str.find(".lua") != std::string::npos)
			{
				pObject->AddScript(path);
			}
		}

		ImGui::EndDragDropTarget();
	}
}

std::string GameObjectInspector::GetFilePathFromExplorer(const char* filter)
{
	OPENFILENAME ofn;
	char fileName[1000] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.nMaxFile = 1000;
	ofn.lpstrFile = fileName;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "";

	if (GetOpenFileName(&ofn))
		return { fileName };

	return {};
}