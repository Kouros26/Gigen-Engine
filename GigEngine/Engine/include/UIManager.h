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

	static int GetSize();
	static UIElement* GetElement(unsigned int i);

	static void SetFocusedElement(UIElement* elem);
	static UIElement* GetFocusedElement();

	static void RemoveElement(UIElement* elem);
	static UIElement* CreateUIElement(UIElement* elem);
	static UIElement* AddUIElement(UIElement* elem);

private:
	inline static UIElement* focusedElement = nullptr;
	inline static std::vector<UIElement*> elements;
	inline static unsigned int MODELLocation;
	inline static unsigned int COLORLocation;
	inline static ShaderProgram shaderProgram;
};
