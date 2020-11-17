#include "Orbit.h"

#include <corecrt_math_defines.h>


Orbit::Orbit(ShaderProgram shaderProgram, unsigned vertsCount, float radius, glm::vec4 color)
	: Object3D(), openGlRender(shaderProgram)
{
	const float angleIncrease = 2 * M_PI / vertsCount;

	std::vector<ColVert> vertices;

	for (unsigned int i = 0; i < vertsCount; i++)
	{
		float angle = i * angleIncrease;

		vertices.emplace_back(radius * static_cast<float>(cos(angle)),
							  radius * static_cast<float>(sin(angle)),
							  0.0f,
							  color.r, color.g, color.b, color.a);
	}

	openGlRender.setBufferData(vertices);
	openGlRender.drawImpl = std::make_unique<ArraysDraw>(GL_LINE_LOOP, vertsCount);
}

Orbit::Orbit(ShaderProgram shaderProgram, unsigned int vertsCount, float radius, OpenGLRender::Texture texture)
	: Object3D(), openGlRender(shaderProgram)
{
	const float angleIncrease = 2 * M_PI / vertsCount;

	std::vector<TexVert> vertices;

	for (unsigned int i = 0; i < vertsCount; i++)
	{
		float angle = i * angleIncrease;

		vertices.emplace_back(radius * static_cast<float>(cos(angle)),
							  radius * static_cast<float>(sin(angle)),
							  0.0f,
							  static_cast<float>(i) / vertsCount, 
							  0.5f);
	}

	openGlRender.setBufferData(vertices);
	openGlRender.addTexture(texture);
	openGlRender.drawImpl = std::make_unique<ArraysDraw>(GL_LINE_LOOP, vertsCount);
}

void Orbit::draw()
{
	openGlRender.draw(modelMat.getTMat());
}
