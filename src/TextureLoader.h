#pragma once

#include <stb_image.h>
#include <stdexcept>
#include <string>

class TextureLoader
{
	stbi_uc* textureData = nullptr;
	
public:
	~TextureLoader()
	{
		if (textureData != nullptr)
			stbi_image_free(textureData);
	}
	
	unsigned char* loadTextureFromPath(const std::string& path, int& width, int& height, int& channelsCount)
	{	
		stbi_set_flip_vertically_on_load(true);
		textureData = stbi_load(path.c_str(), &width, &height, &channelsCount, 0);
		if (textureData == nullptr)
		{
			throw std::logic_error("Error loading texture from: " + path);
		}

		return textureData;
	}
};


