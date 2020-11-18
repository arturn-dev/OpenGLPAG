#pragma once

#include "Object3D.h"
#include "OpenGLRender.h"

class Cylinder : public ColObject3D
{
	unsigned int sectionsCount;
	float height, radius;

	OpenGLRender openGlRender;

	// temporary
protected:
	void createVertexArray(std::vector<ColVert>& verts) override {}
	//
public:
	Cylinder(unsigned sectionsCount, float height, float radius, ShaderProgram shaderProgram, glm::vec4 color);
	Cylinder(const Cylinder&) = delete;
	Cylinder(Cylinder&& other) noexcept;
	Cylinder& operator=(Cylinder&& other) noexcept;

	void draw() override;
};