#pragma once
#include "Shader.h"
#include "Vec3/FVec3.hpp"
#include <vector>

class Line
{
public:
	Line(const lm::FVec3& start, const lm::FVec3& end, const lm::FVec3& color);
	~Line();

	GLuint GetVAO();
	float* GetColor();

private:
	float vertices[6];
	float color[3];

	GLuint VAO;
	GLuint VBO;
};

class Lines
{
public:
	Lines();
	~Lines();

	static void Init();
	static void AddLine(const lm::FVec3& start, const lm::FVec3& end, const lm::FVec3& color = lm::FVec3(1, 0, 0));
	static void DrawLines();
	static void Clear();
private:
	static inline ShaderProgram shaderProgram;
	static inline std::vector<Line*> lines;

	static inline GLint MVPLocation;
	static inline GLint colorLocation;
};