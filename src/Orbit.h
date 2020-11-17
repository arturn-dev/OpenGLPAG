#pragma once

#include "Object3D.h"
#include "ShaderProgram.h"
#include "OpenGLRender.h"

class Orbit : public Object3D
{
	OpenGLRender openGlRender;
	unsigned int vertsCount;
	float radius;

public:
	Orbit(ShaderProgram shaderProgram, unsigned int vertsCount, float radius, glm::vec4 color);
	Orbit(ShaderProgram shaderProgram, unsigned int vertsCount, float radius, OpenGLRender::Texture texture);

	// Temporary
protected:
	void initBuffers() override {}
	void deleteBuffers() override {}
	//
public:
	void draw() override;
};
