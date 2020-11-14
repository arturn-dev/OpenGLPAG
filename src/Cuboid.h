#pragma once

#include "Object3D.h"

class Cuboid
{
	glm::vec3 edgeLenghts;

protected:
	GLint sidesIndices[6];
	GLsizei indicesCounts[6];
	
public:
	Cuboid(glm::vec3 edgeLengths);
	virtual ~Cuboid();

	glm::vec3 getEdgeLenghts() const;
};

class ColCuboid : public Cuboid, public ColObject3D
{
	
protected:
	void internalDraw() const;
	void createVertexArray(std::vector<ColVert>& verts) override;
	
public:
	ColCuboid(GLint modelMatLocation, GLuint aPos, GLuint aCol, glm::vec3 edgeLengths);
	~ColCuboid();

	static ColCuboid* createCube(GLint modelMatLocation, GLuint aPos, GLuint aCol, float edgeLength);

	virtual void draw() const override;
};

class TexCuboid : public Cuboid, public TexObject3D
{
	
protected:
	void internalDraw() const;
	void createVertexArray(std::vector<TexVert>& verts) override;
	
public:
	TexCuboid(GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex, glm::vec3 edgeLengths);
	~TexCuboid();

	static TexCuboid* createCube(GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex, float edgeLength);

	virtual void draw() const override;
};