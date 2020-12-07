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
	unsigned int elementsCount;
	
protected:
	OpenGLRender openGLRender;

public:
	Mesh();
	Mesh(std::vector<T> vertices, 
		 IndexCollection indices,
		 ShaderProgram shaderProgram);
	Mesh(const Mesh& other);
	Mesh(Mesh&& other) noexcept;
	Mesh<T>& operator=(Mesh<T> other);
	friend void swap(Mesh<T>& first, Mesh<T>& second) noexcept
	{
		using std::swap;

		swap(first.openGLRender, second.openGLRender);
		swap(first.elementsCount, second.elementsCount);
	}

	void draw(const glm::mat4);

	OpenGLRender& getOpenGLRender();
	unsigned getElementsCount() const;
};

class TexMesh : public Mesh<Vertex>
{
public:
	TexMesh(VertexCollection vertices, 
		 IndexCollection indices, 
		 ShaderProgram shaderProgram, 
		 const TextureCollection& textures);

	TexMesh(const TexMesh& other)
		: Mesh<Vertex>(other)
	{
	}

	TexMesh(TexMesh&& other) noexcept
		: Mesh<Vertex>()
	{
		swap(*this, other);
	}

	TexMesh& operator=(TexMesh other)
	{
		swap(*this, other);
		return *this;
	}

	friend void swap(TexMesh& lhs, TexMesh& rhs) noexcept
	{
		using std::swap;
		swap(static_cast<Mesh<Vertex>&>(lhs), static_cast<Mesh<Vertex>&>(rhs));
	}

	void addTexture(OpenGLRender::Texture texture);
};