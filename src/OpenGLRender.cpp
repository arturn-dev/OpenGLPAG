#include "OpenGLRender.h"

#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "AssimpModelLoader.h"
#include "Vertex.h"
#include "TextureLoader.h"


OpenGLRender::OpenGLRender(ShaderProgram shaderProgram)
	: OpenGLRender(shaderProgram, nullptr)
{
}

OpenGLRender::OpenGLRender(ShaderProgram shaderProgram, DrawImpl* drawMethod)
	: shaderProgram(shaderProgram), drawImpl(drawMethod)
{
}

OpenGLRender::OpenGLRender(const OpenGLRender& other)
	: OpenGLRender()
{
	vao = other.vao;
	vbo = other.vbo;
	ebo = other.ebo;
	textureInfos = other.textureInfos;
	shaderProgram = other.shaderProgram;
	drawImpl = other.drawImpl;
}

OpenGLRender::OpenGLRender(OpenGLRender&& other) noexcept
	: OpenGLRender()
{
	swap(*this, other);
}

OpenGLRender& OpenGLRender::operator=(OpenGLRender other)
{
	swap(*this, other);
	
	return *this;
}

void swap(OpenGLRender& first, OpenGLRender& second) noexcept
{
	using std::swap;

	swap(first.vao, second.vao);
	swap(first.vbo, second.vbo);
	swap(first.ebo, second.ebo);
	swap(first.textureInfos, second.textureInfos);
	swap(first.shaderProgram, second.shaderProgram);
	swap(first.drawImpl, second.drawImpl);
}

OpenGLRender::OpenGLRender()
{
}

OpenGLRender::~OpenGLRender()
{
}

template <>
void OpenGLRender::setVertexAttribPointers<Vertex>()
{
	glBindVertexArray(*vao);

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

	attribute = shaderProgram.getAttribCol();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's color attribute is unknown in the shader program.");
	}

	glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
						  reinterpret_cast<const void*>(offsetof(Vertex, col)));
	glEnableVertexAttribArray(attribute);

	attribute = shaderProgram.getAttribNorm();
	if (attribute != -1U)
	{
		glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), 
						  reinterpret_cast<const void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(attribute);
	}
	
	glBindVertexArray(0);
}

template<>
void OpenGLRender::setVertexAttribPointers<ColVert>()
{
	glBindVertexArray(*vao);

	GLuint attribute = shaderProgram.getAttribPos();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's position attribute is unknown in the shader program.");
	}
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(ColVert), 
						  reinterpret_cast<const void*>(offsetof(ColVert, pos)));
	glEnableVertexAttribArray(attribute);

	attribute = shaderProgram.getAttribCol();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's color attribute is unknown in the shader program.");
	}

	glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, sizeof(ColVert),
		                      reinterpret_cast<const void*>(offsetof(ColVert, col)));
	glEnableVertexAttribArray(attribute);

	glBindVertexArray(0);
}

template<>
void OpenGLRender::setVertexAttribPointers<TexVert>()
{
	glBindVertexArray(*vao);

	GLuint attribute = shaderProgram.getAttribPos();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's position attribute is unknown in the shader program.");
	}
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(TexVert), 
						  reinterpret_cast<const void*>(offsetof(TexVert, pos)));
	glEnableVertexAttribArray(attribute);

	attribute = shaderProgram.getAttribTex();
	if (attribute == -1U)
	{
		throw std::logic_error("Location of vertex's texture coordinate attribute is unknown in the shader program.");
	}

	glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, sizeof(TexVert),
		                      reinterpret_cast<const void*>(offsetof(TexVert, tex)));
	glEnableVertexAttribArray(attribute);

	glBindVertexArray(0);
}

void OpenGLRender::setIndexBufferData(const std::vector<GLuint>& indices)
{
	ebo = std::shared_ptr<GLuint>(new GLuint, [](auto p) { glDeleteBuffers(1, p); });
	
	glBindVertexArray(*vao);
	
	glGenBuffers(1, ebo.get());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

template <typename T>
void OpenGLRender::setVertexBufferData(const std::vector<T>& verts)
{
	vao = std::shared_ptr<GLuint>(new GLuint, [](auto p) { glDeleteVertexArrays(1, p); });
	vbo = std::shared_ptr<GLuint>(new GLuint, [](auto p) { glDeleteBuffers(1, p); });
	
	glGenVertexArrays(1, vao.get());
	glBindVertexArray(*vao);

	glGenBuffers(1, vbo.get());
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(T), verts.data(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

template <typename T>
void OpenGLRender::setBufferData(const std::vector<T>& verts)
{
	setVertexBufferData(verts);
	setVertexAttribPointers<T>();
}

template <typename T>
void OpenGLRender::setBufferData(const std::vector<T>& verts, const std::vector<GLuint>& indices)
{
	setVertexBufferData(verts);
	setIndexBufferData(indices);
	setVertexAttribPointers<T>();
}

void OpenGLRender::addTexture(Texture texture)
{
	if (textureInfos.size() == 16)
	{
		throw std::logic_error("Maximum number of texture units per object reached.");
	}

	int width, height, channelsCount;

	TextureLoader texLoader;
	unsigned char* textureData = texLoader.loadTextureFromPath(texture.path, width, height, channelsCount);
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(textureData));
	glGenerateMipmap(GL_TEXTURE_2D);

	textureInfos.push_back(
		std::shared_ptr<TextureInfo>(
			new TextureInfo{tex, texture},
			[](auto p){ glDeleteTextures(1, &p->id); }));
}

void OpenGLRender::draw(const glm::mat4 modelMat)
{
	if (drawImpl == nullptr)
	{
		throw std::logic_error("No draw implementation was set.");
	}
	
	shaderProgram.use();
	
	auto i = 0;
	auto diffuseTextureNr = 1;
	auto specularTextureNr = 1;
	for (auto&& textureInfo : textureInfos)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureInfo->id);

		std::string uniformName;
		switch(textureInfo->texture.type)
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

	shaderProgram.setUniformMat4("model", modelMat);
	shaderProgram.setUniformMat4("normModel", glm::transpose(glm::inverse(modelMat)));
	
	glBindVertexArray(*vao);
	drawImpl->draw();

	i = 0;
	for (auto&& textureInfo : textureInfos)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);

		i++;
	}
	
	glBindVertexArray(0);
}

std::vector<OpenGLRender::Texture> OpenGLRender::getTextures() const
{
	std::vector<Texture> textures;
	for (auto&& textureInfo : textureInfos)
	{
		textures.push_back(textureInfo->texture);
	}

	return textures;
}

template void OpenGLRender::setBufferData<Vertex>(const std::vector<Vertex>& verts);
template void OpenGLRender::setBufferData<Vertex>(const std::vector<Vertex>& verts, const std::vector<GLuint>& indices);
template void OpenGLRender::setBufferData<ColVert>(const std::vector<ColVert>& verts);
template void OpenGLRender::setBufferData<ColVert>(const std::vector<ColVert>& verts, const std::vector<GLuint>& indices);
template void OpenGLRender::setBufferData<TexVert>(const std::vector<TexVert>& verts);
template void OpenGLRender::setBufferData<TexVert>(const std::vector<TexVert>& verts, const std::vector<GLuint>& indices);


