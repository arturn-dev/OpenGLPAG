#pragma once

#include <vector>

#include "Object3D.h"
#include "Mesh.h"

typedef std::vector<Mesh> MeshCollection;

class Model : public Object3D
{
	MeshCollection meshes;

public:
	explicit Model(MeshCollection meshes);
	Model(const Model& other) = delete;
	Model(Model&& other) noexcept;
	virtual ~Model();

	Model& operator=(Model&& other) noexcept;

	void draw() override;

	// Temporary methods for compilation correctness

protected:
	void initBuffers() override
	{
		
	}
	
	void deleteBuffers() override
	{
		
	}
};