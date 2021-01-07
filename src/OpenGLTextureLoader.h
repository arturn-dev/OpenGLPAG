#pragma once

#include <list>
#include <string>
#include <glad/glad.h>

#include "OpenGLRender.h"

class OpenGLTextureLoader
{
	unsigned char* loadTextureFromPath(const std::string& path, int& width, int& height, int& channelsCount);

public:
	GLuint generateTexture(OpenGLRender::Texture texture);
	// Texture paths order: right, left, up, down, back, front
	GLuint generateCubemap(std::list<std::string> texturePaths);
};


