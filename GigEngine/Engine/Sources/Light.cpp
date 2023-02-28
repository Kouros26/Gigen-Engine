#include "Light.h"
#include "Application.h"
#include <string>

DirLight::DirLight(float ambient, float diffuse, float specular, lm::FVec3 color)
	:ambient(ambient), diffuse(diffuse), specular(specular)
{
	this->color[0] = color.x;
	this->color[1] = color.y;
	this->color[2] = color.z;
}

void DirLight::SendToShader(const int& pos, const std::string& shaderName)
{
	std::string str = shaderName + "[" + std::to_string(pos) + "].";
	std::string temp = str;

	lm::FVec3 dir = transform.GetFront();
	direction[0] = dir.x;
	direction[1] = dir.y;
	direction[2] = dir.z;

	ShaderProgram& shader = Application::GetMainShader();
	shader.SetVec3(direction, (str.append("direction")).c_str());
	str = temp;
	shader.SetVec3(color, (str.append("color")).c_str());
	str = temp;
	shader.SetFloat(ambient, (str.append("ambient")).c_str());
	str = temp;
	shader.SetFloat(diffuse, (str.append("diffuse")).c_str());
	str = temp;
	shader.SetFloat(specular, (str.append("specular")).c_str());
}

PointLight::PointLight(float ambient, float diffuse, float specular, float constant, float linear, float quadratic, lm::FVec3 color)
	:DirLight(ambient, diffuse, specular, color), constant(constant), linear(linear), quadratic(quadratic)
{
}

void PointLight::SendToShader(const int& pos, const std::string& shaderName)
{
	DirLight::SendToShader(pos, shaderName);

	std::string str = shaderName + "[" + std::to_string(pos) + "].";
	std::string temp = str;

	lm::FVec3 posi = transform.GetPosition();
	position[0] = posi.x;
	position[1] = posi.y;
	position[2] = posi.z;

	ShaderProgram& shader = Application::GetMainShader();
	shader.SetVec3(position, (str.append("position")).c_str());
	str = temp;
	shader.SetFloat(constant, (str.append("ambient")).c_str());
	str = temp;
	shader.SetFloat(linear, (str.append("diffuse")).c_str());
	str = temp;
	shader.SetFloat(quadratic, (str.append("specular")).c_str());
}

SpotLight::SpotLight(float ambient, float diffuse, float specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, lm::FVec3 color)
	:PointLight(ambient, diffuse, specular, constant, linear, quadratic, color), cutOff(cutOff), outerCutOff(outerCutOff)
{
}

void SpotLight::SendToShader(const int& pos, const std::string& shaderName)
{
	PointLight::SendToShader(pos, shaderName);

	std::string str = shaderName + "[" + std::to_string(pos) + "].";
	std::string temp = str;

	ShaderProgram& shader = Application::GetMainShader();
	shader.SetFloat(cutOff, (str.append("cutOff")).c_str());
	str = temp;
	shader.SetFloat(outerCutOff, (str.append("outerCutOff")).c_str());
}