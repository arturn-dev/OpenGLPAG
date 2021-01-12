#pragma once

#include "OpenGLRender.h"
#include "ShaderProgram.h"

class Skybox
{
	OpenGLRender openGlRender;

public:
	Skybox(const std::string& texturesDirPath, ShaderProgram shaderProgram);

	void draw();
};