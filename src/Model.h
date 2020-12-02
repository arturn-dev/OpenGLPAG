#pragma once

#include <vector>

#include "Object3D.h"
#include "Mesh.h"


template <typename T>
class Model : public Object3D
{
	std::vector<T> meshes;

public:
	explicit Model(std::vector<T> meshes);
	Model(const Model<T>& other);
	Model(Model<T>&& other) noexcept;
	Model<T>& operator=(Model<T> other);
	friend void swap(Model<T>& first, Model<T>& second) noexcept
	{
		using std::swap;
		swap(first.modelMat, second.modelMat);
		swap(first.meshes, second.meshes);
	}

	virtual ~Model();

	void draw() override;

	std::vector<T>& getMeshes();
};