#include "Model.h"

#include <utility>

Model::Model(MeshCollection meshes)
	: meshes(std::move(meshes)), modelMat(glm::mat4(1.0f))
{

}

Model::Model(Model&& other) noexcept
	: meshes(other.meshes), modelMat(other.modelMat)
{
	
}

Model::~Model()
{
	for (auto&& mesh : meshes)
	{
		mesh.deleteMesh();
	}
}
