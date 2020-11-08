#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"

typedef std::vector<Mesh> MeshCollection;

class Model
{
	MeshCollection meshes;

public:
	explicit Model(MeshCollection meshes);
	Model(const Model& other) = delete;
	Model(Model&& other) noexcept;
	~Model();

	glm::mat4 modelMat;
};