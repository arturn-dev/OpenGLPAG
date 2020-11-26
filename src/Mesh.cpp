#include "Mesh.h"

#include "Object3D.h"

template <typename T>
Mesh<T>::Mesh()
	: openGLRender(ShaderProgram())
{
}

template <typename T>
Mesh<T>::Mesh(std::vector<T> vertices, 
		   IndexCollection indices, 
	       ShaderProgram shaderProgram)
	: openGLRender(shaderProgram, 
				   new ElementDraw(indices.size())),
	  vertices(std::move(vertices)),
	  indices(std::move(indices))
{
	openGLRender.setBufferData(this->vertices, this->indices);
}

template <typename T>
Mesh<T>::Mesh(Mesh&& other) noexcept
	: openGLRender(std::move(other.openGLRender)),
	  vertices(std::move(other.vertices)),
	  indices(std::move(other.indices))
{
}

template <typename T>
Mesh<T>& Mesh<T>::operator=(Mesh<T>&& other) noexcept
{
	openGLRender = std::move(other.openGLRender);
	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	return *this;
}

TexMesh::TexMesh(VertexCollection vertices, IndexCollection indices, ShaderProgram shaderProgram,
	const TextureCollection& textures)
		: Mesh<Vertex>(std::move(vertices), std::move(indices), std::move(shaderProgram))
{
	for (auto&& tex: textures)
	{
		openGLRender.addTexture(tex);
	}
}

void TexMesh::addTexture(OpenGLRender::Texture texture)
{
	openGLRender.addTexture(texture);
}

template <typename T>
void Mesh<T>::draw(const glm::mat4 modelMat)
{
	openGLRender.draw(modelMat);
}

template <typename T>
void Mesh<T>::deleteMesh()
{
	openGLRender.deleteOpenGlRender();
}

template <typename T>
const OpenGLRender& Mesh<T>::getOpenGLRender() const
{
	return openGLRender;
}

template Mesh<Vertex>;
template Mesh<ColVert>;
template Mesh<TexVert>;