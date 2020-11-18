#pragma once

#include "Object3D.h"
#include "OpenGLRender.h"

class Cylinder : public Object3D
{
	unsigned int sectionsCount;
	float height, radius;

	OpenGLRender openGlRender;

public:
	Cylinder(unsigned sectionsCount, float height, float radius, ShaderProgram shaderProgram, glm::vec4 color);
	Cylinder(const Cylinder&) = delete;
	Cylinder(Cylinder&& other) noexcept;
	Cylinder& operator=(Cylinder&& other) noexcept;

	void draw() override;
};