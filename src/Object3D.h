#pragma once

#include "TMat.h"
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>

struct ColVert
{
	glm::vec3 pos;
	glm::vec4 col;

	static const unsigned int POS_COMP_COUNT = 3;
	static const unsigned int COL_COMP_COUNT = 4;
	static const auto VERT_COMP_COUNT = POS_COMP_COUNT + COL_COMP_COUNT;
	static const auto POS_SIZE = POS_COMP_COUNT * sizeof(float);
	static const auto COL_SIZE = COL_COMP_COUNT * sizeof(float);
	static const auto VERT_SIZE = POS_SIZE + COL_SIZE;

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

	static const unsigned int POS_COMP_COUNT = 3;
	static const unsigned int TEX_COMP_COUNT = 2;
	static const auto VERT_COMP_COUNT = POS_COMP_COUNT + TEX_COMP_COUNT;
	static const auto POS_SIZE = POS_COMP_COUNT * sizeof(float);
	static const auto TEX_SIZE = TEX_COMP_COUNT * sizeof(float);
	static const auto VERT_SIZE = POS_SIZE + TEX_SIZE;

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