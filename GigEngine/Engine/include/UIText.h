#pragma once
#include "UIElement.h"
#include "Shader.h"
#include <string>

class Font;

class UIText : public UIElement
{
public:
	UIText();
	~UIText();

	void SetFont(const std::string& path);
	void SetText(const std::string& t);
	std::string& GetText();
	void Draw() override;

	static void Init();

private:
	inline static ShaderProgram shaderProgram;
	inline static unsigned int ProjLocation;
	inline static unsigned int ColorLocation;

	std::string text = "Hello world!";
	Font* font = nullptr;
};
