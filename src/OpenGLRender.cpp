#include "OpenGLRender.h"

#include <stb_image.h>
#include <stdexcept>
#include "Vertex.h"


void OpenGLRender::freeResources()
{
	for (auto&& texture : textureInfos)
	{
		glDeleteTextures(1, &texture.id);
	}
	textureInfos.clear();

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

OpenGLRender::OpenGLRender(ShaderProgram shaderProgram, DrawImpl* drawMethod)
	: shaderProgram(shaderProgram), drawImpl(drawMethod)
{
}

OpenGLRender::~OpenGLRender()
{
}

template <>
void OpenGLRender::setVertexAttribPointers<Vertex>()
{
	//glBindVertexArray(vao);

	GLuint attribute = shaderProgram.getAttribPos();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's position attribute is unknown in the shader program.");
	}
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
						  reinterpret_cast<const void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(attribute);

	attribute = shaderProgram.getAttribTex();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's texture coordinates attribute is unknown in the shader program.");
	}
	glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
						  reinterpret_cast<const void*>(offsetof(Vertex, tex)));
	glEnableVertexAttribArray(attribute);

	attribute = shaderProgram.getAttribNorm();
	if (attribute != -1U)
	{
		//throw std::logic_error("Location of vertex's normal coordinates attribute is unknown in the shader program.");
		glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
						  reinterpret_cast<const void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(attribute);
	}
	
	
	//glBindVertexArray(0);
}

void OpenGLRender::setIndexBufferData(const std::vector<GLuint>& indices)
{
	glBindVertexArray(vao);
	
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void OpenGLRender::addTextureFromPath(Texture texture)
{
	if (textureInfos.size() == 16)
	{
		throw std::logic_error("Maximum number of texture units per object reached.");
	}

	int width, height, channelsCount;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* textureData = stbi_load(texture.path.c_str(), &width, &height, &channelsCount, 0);
	if (textureData == nullptr)
	{
		throw std::logic_error("Error loading texture from: " + texture.path);
	}
	
	textureInfos.emplace_back();
	auto lastTexInfoIt = (textureInfos.end() - 1);
	lastTexInfoIt->texture = texture;

	glGenTextures(1, &lastTexInfoIt->id);
	glBindTexture(GL_TEXTURE_2D, lastTexInfoIt->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(textureData));
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureData);
}

template <typename T>
void OpenGLRender::setVertexBufferData(const std::vector<T>& verts, const std::vector<GLuint>& indices)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(T), verts.data(), GL_STATIC_DRAW);

	// TODO: remove it and call that code in caller through the method - setIndexBufferData()
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	setVertexAttribPointers<T>();

	glBindVertexArray(0);
}

void OpenGLRender::draw() const
{
	shaderProgram.use();
	
	auto i = 0;
	auto diffuseTextureNr = 1;
	auto specularTextureNr = 1;
	for (auto&& textureInfo : textureInfos)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureInfo.id);

		std::string uniformName;
		switch(textureInfo.texture.type)
		{
			case Texture::TexDiff:
			{
				uniformName = "texture_diffuse" + std::to_string(diffuseTextureNr++);
			}
			break;
			case Texture::TexSpec:
			{
				uniformName = "texture_specular" + std::to_string(specularTextureNr++);
			}
			break;
		}

		glUniform1i(shaderProgram.getUniformLocation(uniformName), i);		

		i++;
	}

	glBindVertexArray(vao);
	drawImpl->draw();

	glBindVertexArray(0);
}

void OpenGLRender::deleteOpenGlRender()
{
	freeResources();
}

template void OpenGLRender::setVertexBufferData<Vertex>(const std::vector<Vertex>& verts, const std::vector<GLuint>& indices);