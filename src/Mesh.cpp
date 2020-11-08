#include "Mesh.h"

void Mesh::initOpenGLRender()
{
	openGLRender.setVertexBufferData<Vertex>(vertices, indices);
}

Mesh::Mesh(VertexCollection vertices, 
		   IndexCollection indices, 
		   const TextureCollection& textures, 
	       ShaderProgram shaderProgram)
	: openGLRender(shaderProgram, 
				   new ElementDraw(indices.size(), reinterpret_cast<const void*>(indices.data()))),
	  vertices(std::move(vertices)),
	  indices(std::move(indices))
{
	openGLRender.setVertexBufferData(this->vertices, this->indices);
	//openGLRender.setIndexBufferData(indices);
	
	for (auto&& tex: textures)
	{
		openGLRender.addTextureFromPath(tex);
	}
}

//Mesh::Mesh(const Mesh& other)
//	: openGLRender(other.openGLRender),
//	  vertices(other.vertices),
//	  indices(other.indices)
//{
//}

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

void Mesh::draw() const
{
	openGLRender.draw();
}

void Mesh::deleteMesh()
{
	openGLRender.deleteOpenGlRender();
}

//Mesh& Mesh::operator=(Mesh other)
//{
//	using std::swap;
//	swap(*this, other);
//	return *this;
//}
