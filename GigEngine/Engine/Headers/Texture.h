#pragma once
#include "IResource.h"

const char* const g_defaultTexturePath = "Resources/Textures/default.png";

class Texture : public IResource
{
public:
	Texture(std::string const& filePath);
	~Texture();

	void Bind();
	static void UnBind();
	bool isValid();

private:
	unsigned int texture;
	void Load();

	bool valid = false;
};