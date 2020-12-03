#pragma once

#include "TMat.h"
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>

struct PosVert
{
	glm::vec3 pos;

	PosVert(aiVector3D pos)
		: pos(*reinterpret_cast<glm::vec3*>(&pos))
	{}
};

struct ColVert
{
	glm::vec3 pos;
	glm::vec4 col;

	ColVert(float x, float y, float z, float r, float g, float b, float a)
		: pos{x, y, z}, col{r, g, b, a}
	{}

	ColVert(aiVector3D pos, aiColor4D col)
		: pos(*reinterpret_cast<glm::vec3*>(&pos)),
		  col(*reinterpret_cast<glm::vec4*>(&col))
	{}
};

struct TexVert
{
	glm::vec3 pos;
	glm::vec2 tex;

	TexVert(float x, float y, float z, float s, float t)
		: pos{x, y, z}, tex{s, t}
	{}

	TexVert(aiVector3D pos, aiVector2D tex)
		: pos(*reinterpret_cast<glm::vec3*>(&pos)),
		  tex(*reinterpret_cast<glm::vec2*>(&tex))
	{}
};

class Object3D
{
public:
	TMat modelMat;

	Object3D() {}
	explicit Object3D(const TMat& modelMat) : modelMat(modelMat) {}
	virtual ~Object3D() {}

	virtual void draw() = 0;
};
