#include "LightSource.h"

DirLight::DirLight(const glm::vec3& direction, const glm::vec3& color, ShaderProgram shaderProgram)
	: direction(direction), color(color), openGlRender(shaderProgram, new ArraysDraw(GL_LINES, 2))
{
	std::vector<ColVert> verts{
		{0.0f, 0.0f, 0.0f, color.r, color.g, color.b, 1.0f},
		{baseVec.x, baseVec.y, baseVec.z, color.r, color.g, color.b, 1.0f},
	};

	openGlRender.setBufferData(verts);
}

void DirLight::draw()
{
	float angle = glm::acos(glm::dot(baseVec, direction) / glm::length(direction));
	glm::vec3 rotVec = glm::cross(baseVec, direction);
	modelMat.setTMat(TMat());
	modelMat.rotate(angle, rotVec);
	
	openGlRender.draw(modelMat.getTMat());
}

glm::vec3 DirLight::getDirection() const
{
	return direction;
}

void DirLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

glm::vec3 DirLight::getColor() const
{
	return color;
}

void DirLight::setColor(const glm::vec3& color)
{
	this->color = color;
}
