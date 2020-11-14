#pragma once

#include "Cuboid.h"
#include "MengerCubeFragmentsCache.h"

class MengerCube
{
protected:
	std::vector<glm::vec3> fragmentsLocalCoords;
	unsigned int n;
	float edgeLength;

public:
	MengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned int n, float edgeLength);
	virtual ~MengerCube();
};

class ColMengerCube : public MengerCube, public ColCuboid
{
public:

	ColMengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned int n, float edgeLength,
				  GLint modelMatLocation, GLuint aPos, GLuint aCol);
	~ColMengerCube();

	void draw() override;
};

class TexMengerCube : public MengerCube, public TexCuboid
{
public:

	TexMengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned int n, float edgeLength,
				  GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex);
	~TexMengerCube();

	void draw() override;
};