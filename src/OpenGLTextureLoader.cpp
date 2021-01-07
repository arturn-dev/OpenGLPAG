#include <stb_image.h>
#include <stdexcept>

#include "OpenGLTextureLoader.h"

unsigned char* OpenGLTextureLoader::loadTextureFromPath(const std::string& path, int& width, int& height,
                                                        int& channelsCount)
{
	stbi_uc* textureData = stbi_load(path.c_str(), &width, &height, &channelsCount, 0);
	if (textureData == nullptr)
	{
		throw std::logic_error("Error loading texture from: " + path);
	}

	return textureData;
}

GLuint OpenGLTextureLoader::generateTexture(OpenGLRender::Texture texture)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channelsCount;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* textureData = loadTextureFromPath(texture.path, width, height, channelsCount);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(textureData));
	stbi_image_free(textureData);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	return tex;
}

GLuint OpenGLTextureLoader::generateCubemap(std::list<std::string> texturePaths)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(false);
	int i = 0;
	for (auto&& texturePath : texturePaths)
	{
		int width, height, channelsCount;
		unsigned char* textureData = loadTextureFromPath(texturePath, width, height, channelsCount);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(textureData));
		stbi_image_free(textureData);
		
		i++;
	}

	return tex;
}

