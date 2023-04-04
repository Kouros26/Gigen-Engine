#include "Texture.h"
#include "STB/stb_image.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(std::string const& filePath)
	:IResource(filePath)
{
	Load();
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::Load()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		valid = true;
	}
	else
	{
		std::cout << "Failed to load texture : " << filePath << std::endl;
	}
	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::isValid()
{
	return valid;
}