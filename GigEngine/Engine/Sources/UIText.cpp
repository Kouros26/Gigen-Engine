#include "UIText.h"
#include "Font.h"
#include "Application.h"
#include "ResourceManager.h"
#include "Renderer.h"

UIText::UIText() : UIElement("Text")
{
	font = ResourceManager::Get<Font>(g_defaultFontPath);
}

UIText::~UIText()
{
}

void UIText::SetFont(const std::string& path)
{
	font = ResourceManager::Get<Font>(path);
}

std::string& UIText::GetText()
{
	return text;
}

void UIText::SetText(const std::string& t)
{
	text = t;
}

void UIText::Draw()
{
	// activate corresponding render state
	shaderProgram.Use();
	GigRenderer::RENDERER.SetUniformValue(ProjLocation, GigRenderer::UniformType::MAT4, &Application::GetWindow().GetOrthoMatrix());
	GigRenderer::RENDERER.SetUniformValue(ColorLocation, GigRenderer::UniformType::VEC3, &GetColor());
	GigRenderer::RENDERER.BindVertexArray(font->GetVAO());

	float x = GetTransform().GetPosition().x * Application::GetWindow().GetVPWidth();
	float y = GetTransform().GetPosition().y * Application::GetWindow().GetVPHeight();

	lm::FVec2 scale = GetTransform().GetSize();
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font->GetCharacter(*c);

		float xpos = x + ch.Bearing.x * scale.x;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale.y;

		float w = ch.Size.x * scale.x;
		float h = ch.Size.y * scale.y;
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
		GigRenderer::RENDERER.BindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		GigRenderer::RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, font->GetVBO());
		GigRenderer::RENDERER.BufferSubData(GigRenderer::BufferType::VERTEX, 0, sizeof(vertices), vertices);
		GigRenderer::RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, 0);
		// render quad
		GigRenderer::RENDERER.DrawArray(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	GigRenderer::RENDERER.BindVertexArray(0);
	GigRenderer::RENDERER.BindTexture(GL_TEXTURE_2D, 0);
	shaderProgram.UnUse();
}

void UIText::Init()
{
	auto* mainVertex = ResourceManager::Get<VertexShader>("Engine/Shaders/UIVertText.vert");
	auto* mainFragment = ResourceManager::Get<FragmentShader>("Engine/Shaders/UIFragText.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking UI shader" << std::endl;

	ProjLocation = GigRenderer::RENDERER.GetUniformLocation(shaderProgram.GetId(), "projection");
	ColorLocation = GigRenderer::RENDERER.GetUniformLocation(shaderProgram.GetId(), "textColor");
}