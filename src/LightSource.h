#pragma once

#include "Object3D.h"
#include "OpenGLRender.h"

class LightSource : public Object3D
{

	
public:
	virtual void draw() = 0;
};

class DirLight : public LightSource
{
	const glm::vec3 baseVec{1.0f, 0.0f, 0.0f};
	glm::vec3 direction;
	glm::vec3 color;

	OpenGLRender openGlRender;
	
public:
	DirLight(const glm::vec3& direction, const glm::vec3& color, ShaderProgram shaderProgram);

	void draw() override;

	glm::vec3 getDirection() const;
	void setDirection(const glm::vec3& direction);
	glm::vec3 getColor() const;
	void setColor(const glm::vec3& color);
};