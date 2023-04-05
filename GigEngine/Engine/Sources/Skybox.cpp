#include "Skybox.h"
#include <ResourceManager.h>
#include "Application.h"

Skybox::Skybox()
{
	SetModel(g_SkyboxModelPath);
	SetTexture(g_SkyboxTexturePath);
	GetTransform().AddRotation(lm::FVec3(180, 0, 0));

	VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/vertSkybox.vert");
	FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/fragSkybox.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking main shader" << std::endl;

	ModelLocation = shaderProgram.GetUniform("model");
	viewProjLocation = shaderProgram.GetUniform("viewProj");
}

Skybox::~Skybox()
{
}

void Skybox::Draw()
{
	shaderProgram.Use();

	if (Application::IsInEditor())
	{
		GetTransform().SetWorldPosition(Application::GetEditorCamera().GetTransform().GetWorldPosition());
	}
	else
	{
		//same but with current camera
	}

	glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, lm::FMat4::ToArray(Application::GetViewProj()));
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, lm::FMat4::ToArray(GetTransform().GetMatrix()));
	UpdateRender();

	shaderProgram.UnUse();
}