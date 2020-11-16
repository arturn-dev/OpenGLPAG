#pragma once

#include "SceneGraphNode.h"

class ShaderProgram;

struct SolarSystemElement
{
	enum ElementType
	{
		ElementOrbit,
		ElementBody
	};
	
	ElementType type;	
	SceneGraphNode* objectNode;
	float orbitRadius;
	float bodyScale;
	glm::vec3 rotAxis;

	SolarSystemElement(ElementType elementType, SceneGraphNode* objectNode)
		: SolarSystemElement(elementType, objectNode, 0.0f, 1.0f, glm::vec3())
	{
	}

	SolarSystemElement(ElementType elementType, SceneGraphNode* objectNode, float orbitRadius)
		: SolarSystemElement(elementType, objectNode, orbitRadius, 1.0f, glm::vec3())
	{
	}

	SolarSystemElement(ElementType elementType, SceneGraphNode* objectNode, float orbitRadius, float bodyScale,
	                   glm::vec3 rotAxis)
		                   : type(elementType), objectNode(objectNode), orbitRadius(orbitRadius), bodyScale(bodyScale), rotAxis(rotAxis)
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