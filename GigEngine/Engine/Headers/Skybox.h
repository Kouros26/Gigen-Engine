#pragma once
#include "Shader.h"
#include "GameObject.h"

const char* const g_SkyboxModelPath = "Resources/Models/Skybox/cube.obj";
const char* const g_SkyboxTexturePath = "Resources/Textures/Skybox/default.png";

class Skybox : GameObject
{
public:
	Skybox();
	~Skybox();

	void Draw();

private:
	GLint viewProjLocation;
	GLint ModelLocation;
	ShaderProgram shaderProgram;
};
