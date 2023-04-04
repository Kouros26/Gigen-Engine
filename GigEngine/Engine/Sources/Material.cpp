#include "Material.h"
#include "Application.h"

Material::Material()
{
}

Material::~Material()
{
}

Material::Material(lm::FVec3 diff, lm::FVec3 amb, lm::FVec3 spec, float shini)
	:shininess(shini)
{
	for (int i = 0; i < 3; i++)
	{
		diffuse[i] = diff[i];
		specular[i] = spec[i];
		ambient[i] = amb[i];
	}
}

void Material::SendToShader()
{
	ShaderProgram& shader = Application::GetMainShader();

	shader.SetVec3(diffuse, "material.diffuse");
	shader.SetVec3(ambient, "material.ambient");
	shader.SetVec3(specular, "material.specular");
	shader.SetFloat(shininess, "material.shininess");
}

void Material::SendDefaultMaterial()
{
	ShaderProgram& shader = Application::GetMainShader();

	float base[3]{ 1, 1, 1 };
	float baseShini = 1.0f;

	shader.SetVec3(base, "material.diffuse");
	shader.SetVec3(base, "material.ambient");
	shader.SetVec3(base, "material.specular");
	shader.SetFloat(baseShini, "material.shininess");
}

void Material::SetDiffuse(lm::FVec3 vec)
{
	for (int i = 0; i < 3; i++)
	{
		diffuse[i] = vec[i];
	}
}

void Material::SetAmbient(lm::FVec3 vec)
{
	for (int i = 0; i < 3; i++)
	{
		ambient[i] = vec[i];
	}
}

void Material::SetSpecular(lm::FVec3 vec)
{
	for (int i = 0; i < 3; i++)
	{
		specular[i] = vec[i];
	}
}

void Material::SetShininess(float shini)
{
	shininess = shini;
}