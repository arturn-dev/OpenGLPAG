#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGLRender.h"
#include "Vertex.h"

typedef std::vector<Vertex> VertexCollection;
typedef std::vector<GLuint> IndexCollection;
typedef std::vector<OpenGLRender::Texture> TextureCollection;

class Mesh
{
	OpenGLRender openGLRender;
	
	VertexCollection vertices;
	IndexCollection indices;

	void initOpenGLRender();

public:
	Mesh(VertexCollection vertices, 
		 IndexCollection indices, 
		 const TextureCollection& textures, 
		 ShaderProgram shaderProgram);
	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other) noexcept;
	//Mesh& operator=(Mesh other);

	void deleteMesh();
};
