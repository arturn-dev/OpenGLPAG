#pragma once

#include <vector>

#include "Mesh.h"

typedef std::vector<Mesh> MeshCollection;

class Model
{
public:
	glm::mat4 modelMat;
	
private:
	MeshCollection meshes;

public:
	explicit Model(MeshCollection meshes);
	Model(const Model& other) = delete;
	Model(Model&& other) noexcept;
	~Model();

	Model& operator=(Model&& other) noexcept;

	void draw() const;
};