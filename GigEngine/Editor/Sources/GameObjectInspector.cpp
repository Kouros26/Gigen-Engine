#include "GameObjectInspector.h"
#include "InterfaceManager.h"
#include "FileDisplay.h"
#include "imgui.h"
#include "Application.h"
#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "GameObjectManager.h"
#include <windows.h>

GameObjectInspector::GameObjectInspector()
{
	InterfaceManager::AddEditorElement(this);
}

GameObjectInspector::~GameObjectInspector()
= default;

void GameObjectInspector::Draw()
{
	height = InterfaceManager::GetHeight() - InterfaceManager::GetClassHeight<FileDisplay>() - g_menuBarSize;
	bool open = true;
	ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width, g_menuBarSize });
	ImGui::SetNextWindowSize({ width, height });

	// noMove et NoCollapse
	ImGui::Begin("Inspector", &open, 4 | 32);

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
	strcpy(name, object->GetName().c_str());

	ImGui::Text("Name"); ImGui::SameLine();

	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("Name", name, 128))
	{
		object->SetName(name);
	}
	ImGui::PopItemWidth();

	ImGui::Separator();

	DrawTransform(object);
	DrawSpecials(object);
	DrawComponents(object);
	DrawModel(object);
}

void GameObjectInspector::DrawTransform(GameObject * pObject)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		lm::FVec3 rot = pObject->GetTransform().GetWorldRotation();
		lm::FVec3 pos = pObject->GetTransform().GetWorldPosition();
		lm::FVec3 scl = pObject->GetTransform().GetWorldScale();

		float translation[] = { pos.x, pos.y, pos.z };
		float scale[] = { scl.x, scl.y, scl.z };
		float rotation[] = { rot.x, rot.y, rot.z };

		ImGui::Text("Position"); ImGui::SameLine();

		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("pos", translation, g_maxStep, g_floatMin, g_floatMax, g_floatFormat))
		{
			pObject->GetTransform().SetWorldPosition(lm::FVec3(translation[0], translation[1], translation[2]));
		}
		ImGui::PopItemWidth();

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("sca", scale, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->GetTransform().SetWorldScale(lm::FVec3(scale[0], scale[1], scale[2]));
		}
		ImGui::PopItemWidth();

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat3("rot", rotation, g_maxStep, -360.0f, 360.0f, g_floatFormat))
		{
			pObject->GetTransform().SetWorldRotation(lm::FVec3(rotation[0], rotation[1], rotation[2]));
		}
		ImGui::PopItemWidth();
	}
}

void GameObjectInspector::DrawModel(GameObject * pObject)
{
	if (ImGui::CollapsingHeader("Model"))
	{
		std::string path;
		if (pObject->GetModel())
		{
			path = pObject->GetModel()->GetFilePath();
		}

		ImGui::Text("Path :"); ImGui::SameLine();
		ImGui::Text(path.c_str());

		if (ImGui::Button("Locate##1"))
		{
			std::string filePath = GetFilePathFromExplorer("3D object \0 *.obj\0 *.OBJ\0 *.fbx\0 *.FBX\0");
			
			if(filePath.length() > 0)
				pObject->SetModel(filePath);
		}

		if (pObject->GetModel()) 
		{
			DrawTexture(pObject);
		}
	}
}

void GameObjectInspector::DrawTexture(GameObject* pObject)
{
	if (ImGui::CollapsingHeader("Texture"))
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
			std::string filePath = GetFilePathFromExplorer("image \0 *.png\0 *.jpeg\0 *.jpg\0");

			if (filePath.length() > 0)
				pObject->SetTexture(filePath);
		}
	}
}

void GameObjectInspector::DrawSpecials(GameObject * pObject)
{
	DirLight* light = dynamic_cast<DirLight*>(pObject);
	if (light)
	{
		DrawLight(pObject);
		return;
	}

	Camera* cam = dynamic_cast<Camera*>(pObject);
	if (cam)
	{
		DrawCamera(cam);
	}
}

void GameObjectInspector::DrawComponents(GameObject * pObject)
{
	//TO DO
}

void GameObjectInspector::DrawLight(GameObject * pObject)
{
	DirLight* dirlight = dynamic_cast<DirLight*>(pObject);
	if (ImGui::CollapsingHeader("Light"))
	{
		float* color = dirlight->GetColor();
		float ambient = dirlight->GetAmbient();
		float diffuse = dirlight->GetDiffuse();
		float specular = dirlight->GetSpecular();

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::ColorEdit3("color", color);
		ImGui::PopItemWidth();

		ImGui::Text("Ambient"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("ambient", &ambient, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetAmbient(ambient);
		}
		ImGui::PopItemWidth();

		ImGui::Text("Diffuse"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("diffuse", &diffuse, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetDiffuse(diffuse);
		}
		ImGui::PopItemWidth();

		ImGui::Text("Specular"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("specular", &specular, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			dirlight->SetSpecular(specular);
		}
		ImGui::PopItemWidth();

		if (PointLight* pointLight = dynamic_cast<PointLight*>(pObject))
		{
			float constant = pointLight->GetConstant();
			float linear = pointLight->GetLinear();
			float quadratic = pointLight->GetQuadratic();

			ImGui::Text("Constant"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::DragFloat("constant", &constant, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetConstant(constant);
			}
			ImGui::PopItemWidth();

			ImGui::Text("Linear"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::DragFloat("linear", &linear, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetLinear(linear);
			}
			ImGui::PopItemWidth();

			ImGui::Text("Quadratic"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::DragFloat("quadratic", &quadratic, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				pointLight->SetQuadratic(quadratic);
			}
			ImGui::PopItemWidth();
		}

		if (SpotLight* spotLight = dynamic_cast<SpotLight*>(pObject))
		{
			float cutOff = spotLight->GetCutOff();
			float outerCutOff = spotLight->GetOuterCutOff();

			ImGui::Text("CutOff"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::DragFloat("cutOff", &cutOff, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				spotLight->SetCutOff(cutOff);
			}
			ImGui::PopItemWidth();

			ImGui::Text("OuterCutOff"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::DragFloat("outerCutOff", &outerCutOff, g_maxStep, 0, g_floatMax, g_floatFormat))
			{
				spotLight->SetOuterCutOff(outerCutOff);
			}
			ImGui::PopItemWidth();
		}
	}
}

void GameObjectInspector::DrawCamera(Camera * pObject)
{
	float fov = pObject->GetFov();
	float tnear = pObject->GetNear();
	float tfar = pObject->GetFar();

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Fov"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("fov", &fov, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetFov(fov);
		}
		ImGui::PopItemWidth();

		ImGui::Text("Near"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("near", &tnear, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetNear(tnear);
		}
		ImGui::PopItemWidth();

		ImGui::Text("Far"); ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("far", &tfar, g_maxStep, 0, g_floatMax, g_floatFormat))
		{
			pObject->SetFar(tfar);
		}
		ImGui::PopItemWidth();
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
		return std::string(fileName);

	return std::string();
}
