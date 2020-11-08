#include "Cuboid.h"

#include <glm/gtc/type_ptr.hpp>

glm::vec3 Cuboid::getEdgeLenghts() const
{
	return edgeLenghts;
}

Cuboid::Cuboid(glm::vec3 edgeLengths)
	: edgeLenghts(edgeLengths)
{
	for (int i = 0; i < 6; ++i)
	{
		sidesIndices[i] = i * 4;
		indicesCounts[i] = 4;
	}
}

Cuboid::~Cuboid()
{
}

void ColCuboid::createVertexArray(std::vector<ColVert>& verts)
{
	const glm::vec3 p = getEdgeLenghts() / glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec4 c(1.0f, 0.0f, 0.0f, 1.0f);

	// front
	verts.push_back(ColVert(-p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));

	// back
	verts.push_back(ColVert( p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));

	c = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	
	// left
	verts.push_back(ColVert(-p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));

	// right
	verts.push_back(ColVert( p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));

	c = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
	// top
	verts.push_back(ColVert(-p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x,  p.y, -p.z, c.r, c.g, c.b, c.a));

	// bottom
	verts.push_back(ColVert(-p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x, -p.y, -p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert(-p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));
	verts.push_back(ColVert( p.x, -p.y,  p.z, c.r, c.g, c.b, c.a));	
}

ColCuboid::ColCuboid(GLint modelMatLocation, GLuint aPos, GLuint aCol, glm::vec3 edgeLengths)
	: Cuboid(edgeLengths), ColObject3D(modelMatLocation)
{
	init(aPos, aCol);
}

ColCuboid::~ColCuboid()
{
}

ColCuboid* ColCuboid::createCube(GLint modelMatLocation, GLuint aPos, GLuint aCol, float edgeLength)
{
	return new ColCuboid(modelMatLocation, aPos, aCol, glm::vec3(edgeLength, edgeLength, edgeLength));
}

void ColCuboid::draw()
{
	glBindVertexArray(getVao());
	glUniformMatrix4fv(getModelMatLocation(), 1, GL_FALSE, glm::value_ptr(modelMat));
	internalDraw();
}

void ColCuboid::internalDraw()
{
	glMultiDrawArrays(GL_TRIANGLE_STRIP, sidesIndices, indicesCounts, sizeof(sidesIndices) / sizeof(GLint));
}

void TexCuboid::createVertexArray(std::vector<TexVert>& verts)
{
	const glm::vec3 p = getEdgeLenghts() / glm::vec3(2.0f, 2.0f, 2.0f);
	const glm::vec2 texRU(1.0f, 1.0f);
	const glm::vec2 texRD(1.0f, 0.0f);
	const glm::vec2 texLD(0.0f, 0.0f);
	const glm::vec2 texLU(0.0f, 1.0f);

	// front
	verts.push_back(TexVert(-p.x, -p.y,  p.z, texLD.x, texLD.y));
	verts.push_back(TexVert( p.x, -p.y,  p.z, texRD.x, texRD.y));
	verts.push_back(TexVert(-p.x,  p.y,  p.z, texLU.x, texLU.y));
	verts.push_back(TexVert( p.x,  p.y,  p.z, texRU.x, texRU.y));

	// back
	verts.push_back(TexVert( p.x, -p.y, -p.z, texLD.x, texLD.y));
	verts.push_back(TexVert(-p.x, -p.y, -p.z, texRD.x, texRD.y));
	verts.push_back(TexVert( p.x,  p.y, -p.z, texLU.x, texLU.y));
	verts.push_back(TexVert(-p.x,  p.y, -p.z, texRU.x, texRU.y));

	// left
	verts.push_back(TexVert(-p.x, -p.y, -p.z, texLD.x, texLD.y));
	verts.push_back(TexVert(-p.x, -p.y,  p.z, texRD.x, texRD.y));
	verts.push_back(TexVert(-p.x,  p.y, -p.z, texLU.x, texLU.y));
	verts.push_back(TexVert(-p.x,  p.y,  p.z, texRU.x, texRU.y));

	// right
	verts.push_back(TexVert( p.x, -p.y,  p.z, texLD.x, texLD.y));
	verts.push_back(TexVert( p.x, -p.y, -p.z, texRD.x, texRD.y));
	verts.push_back(TexVert( p.x,  p.y,  p.z, texLU.x, texLU.y));
	verts.push_back(TexVert( p.x,  p.y, -p.z, texRU.x, texRU.y));

	// top
	verts.push_back(TexVert(-p.x,  p.y,  p.z, texLD.x, texLD.y));
	verts.push_back(TexVert( p.x,  p.y,  p.z, texRD.x, texRD.y));
	verts.push_back(TexVert(-p.x,  p.y, -p.z, texLU.x, texLU.y));
	verts.push_back(TexVert( p.x,  p.y, -p.z, texRU.x, texRU.y));

	// bottom
	verts.push_back(TexVert(-p.x, -p.y, -p.z, texLD.x, texLD.y));
	verts.push_back(TexVert( p.x, -p.y, -p.z, texRD.x, texRD.y));
	verts.push_back(TexVert(-p.x, -p.y,  p.z, texLU.x, texLU.y));
	verts.push_back(TexVert( p.x, -p.y,  p.z, texRU.x, texRU.y));
}

TexCuboid::TexCuboid(GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex,
	glm::vec3 edgeLengths)
		: Cuboid(edgeLengths), TexObject3D(modelMatLocation, texturePath)
{
	init(aPos, aTex);
}

TexCuboid::~TexCuboid()
{
}

TexCuboid* TexCuboid::createCube(GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex,
	float edgeLength)
{
	return new TexCuboid(modelMatLocation, texturePath, aPos, aTex, glm::vec3(edgeLength, edgeLength, edgeLength));
}

void TexCuboid::draw()
{
	glBindVertexArray(getVao());
	glUniformMatrix4fv(getModelMatLocation(), 1, GL_FALSE, glm::value_ptr(modelMat));
	internalDraw();
}

void TexCuboid::internalDraw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMultiDrawArrays(GL_TRIANGLE_STRIP, sidesIndices, indicesCounts, sizeof(sidesIndices) / sizeof(GLint));
}
