#include "UIImage.h"
#include "Renderer.h"
#include "Application.h"
#include "ResourceManager.h"

using namespace GigRenderer;

UIImage::UIImage() : UIElement("Image")
{
	texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
	SetIsImage(1);
	GetTransform().SetSize({ 100 });
	RENDERER.LoadUIImage(this);
}

UIImage::~UIImage()
{
}

unsigned int& UIImage::GetVAO()
{
	return VAO;
}

unsigned int& UIImage::GetVBO()
{
	return VBO;
}

void UIImage::SetTexture(const std::string& path)
{
	texture = ResourceManager::Get<Texture>(path);
}

void UIImage::Draw()
{
	GigRenderer::RENDERER.BindVertexArray(VAO);

	lm::FVec2 scale = GetTransform().GetSize();
	// iterate through all characters
	std::string::const_iterator c;

	float xpos = GetTransform().GetPosition().x;
	float ypos = GetTransform().GetPosition().y;

	float w = scale.x;
	float h = scale.y;
	// update VBO for each character
	float vertices[6][4] = {
		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos,     ypos,       0.0f, 1.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },

		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },
		{ xpos + w, ypos + h,   1.0f, 0.0f }
	};
	// render glyph texture over quad
	texture->Bind();
	// update content of VBO memory
	RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, VBO);
	RENDERER.BufferSubData(GigRenderer::BufferType::VERTEX, 0, sizeof(vertices), vertices);
	RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, 0);

	RENDERER.DrawArray(GL_TRIANGLES, 0, 6);

	RENDERER.BindVertexArray(0);
	RENDERER.BindTexture(GL_TEXTURE_2D, 0);
}