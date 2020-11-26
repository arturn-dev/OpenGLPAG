#include "Model.h"

#include <utility>

template <typename T>
Model<T>::Model(std::vector<T> meshes)
	: Object3D(TMat()), meshes(std::move(meshes))
{

}

template <typename T>
Model<T>::Model(Model&& other) noexcept
	: Object3D(other.modelMat), meshes(std::move(other.meshes))
{
	
}

template <typename T>
Model<T>::~Model()
{
	for (auto&& mesh : meshes)
	{
		mesh.deleteMesh();
	}
}

template <typename T>
Model<T>& Model<T>::operator=(Model<T>&& other) noexcept
{
	meshes = std::move(other.meshes);
	modelMat = other.modelMat;

	return *this;
}

template <typename T>
void Model<T>::draw()
{
	for (auto&& mesh : meshes)
	{
		mesh.draw(modelMat.getTMat());
	}
}

template <typename T>
std::vector<T>& Model<T>::getMeshes()
{
	return meshes;
}

template Model<TexMesh>;
template Model<Mesh<Vertex>>;
template Model<Mesh<ColVert>>;
template Model<Mesh<TexVert>>;