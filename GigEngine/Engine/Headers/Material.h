#pragma once
#include "Vec4/FVec4.hpp"

class Material
{
public:
	Material();
	Material(lm::FVec3 diff, lm::FVec3 amb, lm::FVec3 spec, float shini);
	~Material();

	void SendToShader();
	static void SendDefaultMaterial();

	void SetDiffuse(lm::FVec3 vec);
	void SetAmbient(lm::FVec3 vec);
	void SetSpecular(lm::FVec3 vec);
	void SetShininess(float shini);

private:
	float diffuse[3];
	float ambient[3];
	float specular[3];

	float shininess = 1.0f;
};