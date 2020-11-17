#pragma once

#include "SceneGraphNode.h"

class ShaderProgram;

struct SolarSystemElement
{
	enum ElementType
	{
		ElementOrbiting,
		ElementOrbit,
		ElementBody
	};
	
	ElementType type;	
	SceneGraphNode* objectNode;
	float orbitRadius;
	float bodyScale;
	float tiltDeg;
	bool needOrientationFix;

	SolarSystemElement(ElementType elementType, SceneGraphNode* objectNode, float orbitRadius = 0.0f, float bodyScale = 1.0f,
	                   float tiltDeg = 0, bool needOrientationFix = false)
		                   : type(elementType), objectNode(objectNode), orbitRadius(orbitRadius), bodyScale(bodyScale), tiltDeg(tiltDeg), needOrientationFix(needOrientationFix)
	{
	}
};

class SolarSystem : public Object3D
{
	std::vector<SolarSystemElement> solarSystemElements;
	SceneGraphNode rootNode;

	const float orbitalSpeedFactor = 0.2f;
	const float selfAxisRotationSpeedFactor = 0.02f;
	
	void initElements();

	// Temporary
protected:
	void initBuffers() override {}
	void deleteBuffers() override {}
	//
public:
	SolarSystem(const ShaderProgram& shaderProgram, TMat modelMat = glm::mat4(1.0f));

	void animate();
	void draw() override;
};