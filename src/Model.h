#pragma once

#include <vector>

#include "Object3D.h"
#include "Mesh.h"

//typedef std::vector<Mesh> MeshCollection;

template <typename T>
class Model : public Object3D
{
	std::vector<T> meshes;

public:
	explicit Model(std::vector<T> meshes);
	Model(const Model& other) = delete;
	Model(Model&& other) noexcept;
	virtual ~Model();

	Model<T>& operator=(Model<T>&& other) noexcept;

	void draw() override;

	std::vector<T>& getMeshes();
};