#include "UIText.h"
#include "Font.h"
#include "Application.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace GigRenderer;

UIText::UIText() : UIElement("Text")
{
	font = ResourceManager::Get<Font>(g_defaultFontPath);
	GetTransform().SetSize({ 1 });
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
	RENDERER.BindVertexArray(font->GetVAO());

	float x = GetTransform().GetPosition().x;
	float y = GetTransform().GetPosition().y;

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
		RENDERER.BindTexture(GL_TEXTURE_2D, ch.TextureID);
		RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, font->GetVBO());
		RENDERER.BufferSubData(GigRenderer::BufferType::VERTEX, 0, sizeof(vertices), vertices);
		RENDERER.BindBuffer(GigRenderer::BufferType::VERTEX, 0);
		RENDERER.DrawArray(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	RENDERER.BindVertexArray(0);
	RENDERER.BindTexture(GL_TEXTURE_2D, 0);
}