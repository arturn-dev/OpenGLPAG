#include "Model.h"

#include <utility>

Model::Model(MeshCollection meshes)
	: Object3D(TMat()), meshes(std::move(meshes))
{

}

Model::Model(Model&& other) noexcept
	: Object3D(other.modelMat), meshes(std::move(other.meshes))
{
	
}

Model::~Model()
{
	for (auto&& mesh : meshes)
	{
		mesh.deleteMesh();
	}
}

Model& Model::operator=(Model&& other) noexcept
{
	meshes = std::move(other.meshes);
	modelMat = other.modelMat;

	return *this;
}

void Model::draw()
{
	for (auto&& mesh : meshes)
	{
		mesh.draw(modelMat.getTMat());
	}
}
