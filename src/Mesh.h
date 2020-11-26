#pragma once

#include <vector>

#include "OpenGLRender.h"
#include "Vertex.h"

typedef std::vector<Vertex> VertexCollection;
typedef std::vector<GLuint> IndexCollection;
typedef std::vector<OpenGLRender::Texture> TextureCollection;

template <typename T>
class Mesh
{
protected:
	OpenGLRender openGLRender;
	
	std::vector<T> vertices;
	IndexCollection indices;

public:
	Mesh();
	Mesh(std::vector<T> vertices, 
		 IndexCollection indices,
		 ShaderProgram shaderProgram);
	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other) noexcept;
	Mesh<T>& operator=(Mesh<T>&& other) noexcept;

	//void addTexture(OpenGLRender::Texture texture);

	void draw(const glm::mat4);
	void deleteMesh();

	const OpenGLRender& getOpenGLRender() const;
};

class TexMesh : public Mesh<Vertex>
{
public:
	TexMesh(VertexCollection vertices, 
		 IndexCollection indices, 
		 ShaderProgram shaderProgram, 
		 const TextureCollection& textures);

	void addTexture(OpenGLRender::Texture texture);
};