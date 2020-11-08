#pragma once

#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;

	Vertex(aiVector3D aiVec3Pos, aiVector3D aiVec3Norm, aiVector2D aiVec2Tex)
		: pos(*reinterpret_cast<glm::vec3*>(&aiVec3Pos)),
		  normal(*reinterpret_cast<glm::vec3*>(&aiVec3Norm)),
		  tex(*reinterpret_cast<glm::vec2*>(&aiVec2Tex))
	{}
};