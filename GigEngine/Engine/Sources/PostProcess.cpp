#include "PostProcess.h"
#include "ResourceManager.h"
#include "Application.h"
#include "Renderer.h"

using namespace GigRenderer;

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::Init()
{
	width = Application::GetWindow().GetVPWidth();
	height = Application::GetWindow().GetVPHeight();
	RENDERER.InitPostProcess();
	InitShader();
}

void PostProcess::Clear()
{
	RENDERER.ClearPostProcess();
	RENDERER.DeleteProgram(shaderProgram.GetId());
}

void PostProcess::BeginRender()
{
	RENDERER.BeginRenderPostProcess();
}

void PostProcess::EndRender()
{
	RENDERER.EndRenderPostProcess();
}

void PostProcess::Render(float time)
{
	shaderProgram.Use();

	RENDERER.SetUniformValue(timeLocation, UniformType::FLOAT, &time);
	RENDERER.SetUniformValue(confuseLocation, UniformType::INT, &confuse);
	RENDERER.SetUniformValue(chaosLocation, UniformType::INT, &chaos);
	RENDERER.SetUniformValue(shakeLocation, UniformType::INT, &shake);

	RENDERER.BindTexture(GL_TEXTURE_2D, texture);
	RENDERER.BindVertexArray(VAO);
	RENDERER.DrawArray(GL_TRIANGLES, 0, 6);
	RENDERER.BindVertexArray(0);

	shaderProgram.UnUse();
}

void PostProcess::SetVPSize()
{
	width = Application::GetWindow().GetVPWidth();
	height = Application::GetWindow().GetVPHeight();
}

int& PostProcess::GetWidth()
{
	return width;
}

int& PostProcess::GetHeight()
{
	return height;
}

unsigned int& PostProcess::GetMSFBO()
{
	return MSFBO;
}

unsigned int& PostProcess::GetFBO()
{
	return FBO;
}

unsigned int& PostProcess::GetRBO()
{
	return RBO;
}

unsigned int& PostProcess::GetVAO()
{
	return VAO;
}

unsigned int& PostProcess::GetTexture()
{
	return texture;
}

void PostProcess::InitShader()
{
	auto* mainVertex = ResourceManager::Get<VertexShader>("Engine/Shaders/postProcess.vert");
	auto* mainFragment = ResourceManager::Get<FragmentShader>("Engine/Shaders/postProcess.frag");

	if (!shaderProgram.Link(mainVertex, mainFragment))
		std::cout << "Error linking drawLine shader" << std::endl;

	confuseLocation = shaderProgram.GetUniform("confuse");
	chaosLocation = shaderProgram.GetUniform("chaos");
	timeLocation = shaderProgram.GetUniform("time");
	shakeLocation = shaderProgram.GetUniform("shake");

	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right
	};

	RENDERER.SetUniformValue(shaderProgram.GetId(), "offsets", UniformType::FLOAT2V, offsets, 9);

	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	RENDERER.SetUniformValue(shaderProgram.GetId(), "edge_kernel", UniformType::INTV, edge_kernel, 9);

	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	RENDERER.SetUniformValue(shaderProgram.GetId(), "blur_kernel", UniformType::FLOATV, blur_kernel, 9);
}