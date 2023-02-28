#pragma once
#include "GameObject.h"
#include "Vec3/FVec3.hpp"

const std::string dirLightShaderName = "dirLights";
const std::string pointLightShaderName = "pointLights";
const std::string spotLightShaderName = "spotLights";

class DirLight : public GameObject
{
public:
	DirLight(float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f, lm::FVec3 color = lm::FVec3(1));

	virtual void SendToShader(const int& pos, const std::string& shaderName);

private:
	float direction[3];
	float color[3];

	float ambient;
	float diffuse;
	float specular;
};

class PointLight : public DirLight
{
public:
	PointLight(float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f,
		float constant = 0.5f, float linear = 0.5f, float quadratic = 0.5f,
		lm::FVec3 color = lm::FVec3(1));

	virtual void SendToShader(const int& pos, const std::string& shaderName) override;

private:
	float position[3];

	float constant;
	float linear;
	float quadratic;
};

class SpotLight : public PointLight
{
public:
	SpotLight(float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f,
		float constant = 0.5f, float linear = 0.5f, float quadratic = 0.5f,
		float cutOff = 45, float outerCutOff = 90,
		lm::FVec3 color = lm::FVec3(1));

	virtual void SendToShader(const int& pos, const std::string& shaderName) override;

private:
	float cutOff;
	float outerCutOff;
};