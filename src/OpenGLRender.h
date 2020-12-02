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

	std::shared_ptr<DrawImpl> drawImpl;
	
private:
	struct TextureInfo
	{
		GLuint id;
		Texture texture;
	};
	
	std::shared_ptr<GLuint> vao, vbo, ebo;
	std::vector<std::shared_ptr<TextureInfo>> textureInfos; // TODO: Share the textures too.
	ShaderProgram shaderProgram;
	
	OpenGLRender();
	template <typename T>
	void setVertexAttribPointers();
	template <typename T>
	void setVertexBufferData(const std::vector<T>& verts);
	void setIndexBufferData(const std::vector<GLuint>& indices);

public:
	explicit OpenGLRender(ShaderProgram shaderProgram);
	OpenGLRender(ShaderProgram shaderProgram, DrawImpl* drawMethod);
	
	OpenGLRender(const OpenGLRender& other);
	OpenGLRender(OpenGLRender&& other) noexcept;
	OpenGLRender& operator=(OpenGLRender other);
	friend void swap(OpenGLRender& first, OpenGLRender& second) noexcept;
	
	~OpenGLRender();	

	template <typename T>
	void setBufferData(const std::vector<T>& verts);		
	template <typename T>
	void setBufferData(const std::vector<T>& verts, const std::vector<GLuint>& indices);
	
	void addTexture(Texture texture);
	void draw(const glm::mat4 modelMat);

	std::vector<Texture> getTextures() const;
};

