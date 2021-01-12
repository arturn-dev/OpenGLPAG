#pragma once

#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
	glm::vec4 col;

	Vertex(aiVector3D aiVec3Pos, aiVector3D aiVec3Norm, aiVector2D aiVec2Tex, aiColor4D aiCol)
		: pos(*reinterpret_cast<glm::vec3*>(&aiVec3Pos)),
		  normal(*reinterpret_cast<glm::vec3*>(&aiVec3Norm)),
		  tex(*reinterpret_cast<glm::vec2*>(&aiVec2Tex)),
		  col(*reinterpret_cast<glm::vec4*>(&aiCol))
	{}
};

struct PosVert
{
	glm::vec3 pos;
	glm::vec3 normal;

	PosVert(aiVector3D pos, aiVector3D norm = aiVector3D{0.0f})
		: pos(*reinterpret_cast<glm::vec3*>(&pos)),
		  normal(*reinterpret_cast<glm::vec3*>(&norm))
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