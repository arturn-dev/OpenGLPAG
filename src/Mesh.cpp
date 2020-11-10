#include "Mesh.h"

//void Mesh::initOpenGLRender()
//{
//	openGLRender.setVertexBufferData<Vertex>(vertices, indices);
//}

Mesh::Mesh(VertexCollection vertices, 
		   IndexCollection indices, 
		   const TextureCollection& textures, 
	       ShaderProgram shaderProgram)
	: openGLRender(shaderProgram, 
				   new ElementDraw(indices.size())),
	  vertices(std::move(vertices)),
	  indices(std::move(indices))
{
	openGLRender.setBufferData(this->vertices, this->indices);
	
	for (auto&& tex: textures)
	{
		openGLRender.addTextureFromPath(tex);
	}
}

Mesh::Mesh(Mesh&& other) noexcept
	: openGLRender(std::move(other.openGLRender)),
	  vertices(std::move(other.vertices)),
	  indices(std::move(other.indices))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	openGLRender = std::move(other.openGLRender);
	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	return *this;
}

void Mesh::draw(const glm::mat4 modelMat) const
{
	openGLRender.draw(modelMat);
}

void Mesh::deleteMesh()
{
	openGLRender.deleteOpenGlRender();
}