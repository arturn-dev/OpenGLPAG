#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include "ShaderProgram.h"
#include "DrawImpl.h"

class OpenGLRender
{
public:

	struct Texture
	{
		enum Type
		{
			TexDiff,
			TexSpec
		} type;

		std::string path;
	};

private:
	struct TextureInfo
	{
		GLuint id;
		Texture texture;
	};
	
	GLuint vao, vbo, ebo;
	std::vector<TextureInfo> textureInfos;
	ShaderProgram shaderProgram;
	std::unique_ptr<DrawImpl> drawImpl;

	template <typename T>
	void setVertexAttribPointers();
	void freeResources();

public:
	OpenGLRender(ShaderProgram shaderProgram, DrawImpl* drawMethod);

	OpenGLRender(const OpenGLRender& other) = delete;
	/*	: vao(other.vao),
		  vbo(other.vbo),
		  ebo(other.ebo),
		  textureInfos(other.textureInfos),
		  shaderProgram(other.shaderProgram),
		  drawImpl(other.drawImpl.get())
	{
	}*/

	OpenGLRender(OpenGLRender&& other) noexcept // TODO: move implementation to cpp file
		: vao(other.vao),
		  vbo(other.vbo),
		  ebo(other.ebo),
		  textureInfos(std::move(other.textureInfos)),
		  shaderProgram(std::move(other.shaderProgram)),
		  drawImpl(std::move(other.drawImpl))
	{
	}

	//OpenGLRender& operator=(OpenGLRender other)
	//{
	//	using std::swap;
	//	swap(*this, other);
	//	return *this;
	//}

	OpenGLRender& operator=(OpenGLRender&& other)
	{
		vao = other.vao;		
		vbo = other.vbo;
		ebo = other.ebo;
		textureInfos = std::move(other.textureInfos);
		shaderProgram = std::move(other.shaderProgram);
		drawImpl = std::move(other.drawImpl);

		return *this;
	}

	~OpenGLRender();

	template <typename T>
	void setVertexBufferData(const std::vector<T>& verts, const std::vector<GLuint>& indices);
	void setIndexBufferData(const std::vector<GLuint>& indices);
	void addTextureFromPath(Texture texture);
	void draw() const;

	void deleteOpenGlRender();
};