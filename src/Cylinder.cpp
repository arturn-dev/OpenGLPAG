#include "Cylinder.h"

#include <corecrt_math_defines.h>
#include <glm/glm.hpp>

Cylinder::Cylinder(unsigned sectionsCount, float height, float radius, ShaderProgram shaderProgram, glm::vec4 color)
	: ColObject3D(), sectionsCount(sectionsCount), height(height), radius(radius), openGlRender(shaderProgram)
													
{
	const float angleIncrease = 2 * M_PI / sectionsCount;

	std::vector<ColVert> vertices;

	int meshVertsCount = (sectionsCount + 1) * 2;
	
	openGlRender.drawImpl = std::unique_ptr<ArraysMultiDraw>(
		new ArraysMultiDraw({0, meshVertsCount, meshVertsCount * 2}, {meshVertsCount, meshVertsCount, meshVertsCount}));

	// Create sides
	for (unsigned int i = 0; i <= sectionsCount; i++)
	{
		float angle = i * angleIncrease;

		vertices.emplace_back(radius * static_cast<float>(cos(angle)), 
							  radius * static_cast<float>(sin(angle)),
							  height / 2,
							  color.r, color.g, color.b, color.a);

		vertices.emplace_back(radius * static_cast<float>(cos(angle)), 
							  radius * static_cast<float>(sin(angle)),
							  -height / 2,
							  color.r, color.g, color.b, color.a);
	}

	// Create bases
	for (unsigned int i = 0; i <= sectionsCount; i++)
	{
		float angle = i * angleIncrease;
		
		vertices.emplace_back(0, 
							  0,
							  height / 2,
							  color.r, color.g, color.b, color.a);

		vertices.emplace_back(radius * static_cast<float>(cos(angle)), 
							  radius * static_cast<float>(sin(angle)),
							  height / 2,
							  color.r, color.g, color.b, color.a);
	}
	for (unsigned int i = 0; i <= sectionsCount; i++)
	{
		float angle = i * angleIncrease;
		
		vertices.emplace_back(radius * static_cast<float>(cos(angle)), 
							  radius * static_cast<float>(sin(angle)),
							  -height / 2,
							  color.r, color.g, color.b, color.a);

		vertices.emplace_back(0, 
							  0,
							  -height / 2,
							  color.r, color.g, color.b, color.a);

		
	}

	openGlRender.setBufferData(vertices);
}

Cylinder::Cylinder(Cylinder&& other) noexcept
	: openGlRender(std::move(other.openGlRender)),
	  sectionsCount(other.sectionsCount), height(other.radius), radius(other.radius)
{
	
}

Cylinder& Cylinder::operator=(Cylinder&& other) noexcept
{
	openGlRender = std::move(other.openGlRender);
	sectionsCount = other.sectionsCount;
	height = other.height;
	radius = other.radius;

	return *this;
}

void Cylinder::draw()
{
	openGlRender.draw(modelMat.getTMat());
}

