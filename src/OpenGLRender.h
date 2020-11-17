#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
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

	std::unique_ptr<DrawImpl> drawImpl;
	
private:
	struct TextureInfo
	{
		GLuint id;
		Texture texture;
	};
	
	GLuint vao, vbo, ebo;
	std::vector<TextureInfo> textureInfos;
	ShaderProgram shaderProgram;
	

	template <typename T>
	void setVertexAttribPointers();
	template <typename T>
	void setVertexBufferData(const std::vector<T>& verts);
	void setIndexBufferData(const std::vector<GLuint>& indices);
	void freeResources();

public:
	explicit OpenGLRender(ShaderProgram shaderProgram);
	OpenGLRender(ShaderProgram shaderProgram, DrawImpl* drawMethod);
	OpenGLRender(const OpenGLRender& other) = delete;
	OpenGLRender(OpenGLRender&& other) noexcept;
	~OpenGLRender();

	OpenGLRender& operator=(OpenGLRender&& other) noexcept;

	template <typename T>
	void setBufferData(const std::vector<T>& verts);		
	template <typename T>
	void setBufferData(const std::vector<T>& verts, const std::vector<GLuint>& indices);
	
	void addTexture(Texture texture);
	void draw(const glm::mat4 modelMat);

	void deleteOpenGlRender();

	std::vector<Texture> getTextures() const;
};

