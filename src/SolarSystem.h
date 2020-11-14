#pragma once

#include "SceneGraphNode.h"

class ShaderProgram;

struct CelestialBody
{
	SceneGraphNode& objectNode;
	float orbitRadius;
	float bodyScale;
	glm::vec3 rotAxis;
};

class SolarSystem : public Object3D
{
	std::vector<CelestialBody> celestialBodies;

	void initCelestialBodies();
	
public:
	SolarSystem(ShaderProgram shaderProgram, TMat modelMat = glm::mat4(1.0f));

	void animate();
	void draw();
};