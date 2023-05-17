#pragma once
#include "Shader.h"

class PostProcess
{
public:
	PostProcess();
	~PostProcess();

	static void Init();
	static void Clear();
	static void BeginRender();
	static void EndRender();
	static void Render(float time);

	static int& GetWidth();
	static int& GetHeight();
	static unsigned int& GetMSFBO();
	static unsigned int& GetFBO();
	static unsigned int& GetRBO();
	static unsigned int& GetVAO();
	static unsigned int& GetTexture();

private:
	inline static bool confuse, chaos, shake;
	inline static int width, height;

	inline static unsigned int confuseLocation, chaosLocation, shakeLocation, timeLocation;

	inline static unsigned int texture;
	inline static unsigned int MSFBO, FBO;
	inline static unsigned int RBO;
	inline static unsigned int VAO;

	static void InitShader();

	inline static ShaderProgram shaderProgram;
};