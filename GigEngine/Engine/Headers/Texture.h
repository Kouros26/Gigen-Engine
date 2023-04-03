#pragma once
#include "IResource.h"

class Texture : public IResource
{
public:
	Texture(std::string const& filePath);
	~Texture();

	void Bind();
	static void UnBind();

private:
	unsigned int texture;
	void Load();
};