#include "Mesh.h"

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
		openGLRender.addTexture(tex);
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

void Mesh::addTexture(OpenGLRender::Texture texture)
{
	openGLRender.addTexture(texture);
}

void Mesh::draw(const glm::mat4 modelMat)
{
	openGLRender.draw(modelMat);
}

void Mesh::deleteMesh()
{
	openGLRender.deleteOpenGlRender();
}

const OpenGLRender& Mesh::getOpenGLRender() const
{
	return openGLRender;
}
