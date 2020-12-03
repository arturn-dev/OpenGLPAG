#include "Model.h"

#include <utility>

template <typename T>
Model<T>::Model()
{
}

template <typename T>
Model<T>::Model(std::vector<T> meshes)
	: Object3D(TMat()), meshes(std::move(meshes))
{

}

template <typename T>
Model<T>::Model(const Model<T>& other)
	: Object3D(other.modelMat), meshes(other.meshes)
{
}

template <typename T>
Model<T>::Model(Model<T>&& other) noexcept
	: Object3D()
{
	swap(*this, other);
}

template <typename T>
Model<T>& Model<T>::operator=(Model<T> other)
{
	swap(*this, other);
	
	return *this;
}

template <typename T>
Model<T>::~Model()
{
	
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
template Model<Mesh<PosVert>>;