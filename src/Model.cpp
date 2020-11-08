#include "Model.h"

#include <utility>

Model::Model(MeshCollection meshes)
	: meshes(std::move(meshes)), modelMat(glm::mat4(1.0f))
{

}

Model::Model(Model&& other) noexcept
	: meshes(std::move(other.meshes)), modelMat(other.modelMat)
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

void Model::draw() const
{
	for (auto&& mesh : meshes)
	{
		mesh.draw();
	}
}
