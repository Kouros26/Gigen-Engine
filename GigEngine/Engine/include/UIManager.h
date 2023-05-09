#pragma once
#include "UIElement.h"
#include "Shader.h"
#include <vector>

class UIManager
{
public:
	UIManager();
	~UIManager();

	static void Init();
	static void AddImageElement();
	static void AddTextElement();
	static void DrawUI();

private:
	inline static std::vector<UIElement*> elements;
	inline static unsigned int PROJLocation;
	inline static unsigned int MODELLocation;
	inline static unsigned int COLORLocation;
	inline static ShaderProgram shaderProgram;
};
