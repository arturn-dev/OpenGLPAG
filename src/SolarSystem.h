#pragma once

#include "SceneGraphNode.h"
#include "AssimpModelLoader.h"

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
	float rotationSpeed;
	float bodyScale;
	float tiltDeg;
	bool needOrientationFix;

	SolarSystemElement(ElementType elementType, SceneGraphNode* objectNode, float orbitRadius = 0.0f, float rotationSpeed = 1.0f,
					   float bodyScale = 1.0f, float tiltDeg = 0, bool needOrientationFix = false)
		                   : type(elementType), objectNode(objectNode), orbitRadius(orbitRadius), rotationSpeed(rotationSpeed),
							 bodyScale(bodyScale), tiltDeg(tiltDeg), needOrientationFix(needOrientationFix)
	{
	}
};

class SolarSystem : public Object3D
{
	std::vector<SolarSystemElement> solarSystemElements;
	unsigned int cylMinRes, cylMaxRes;
	unsigned long long cylindersNodesIdx;
	SceneGraphNode rootNode;

	const float orbitalSpeedFactor = 0.2f;
	
	void initElements();

	template <typename T>
	unsigned long long addObjectToParent(int parentAttachNodeIdx, T&& childObject, ShaderProgram shaderProgram,
	                                        float orbitRadius, float rotationSpeed, float bodyScale, float tiltDeg, bool needOrientationFix, glm::vec4 color);
	unsigned long long addTexModelToParent(int parentAttachNodeIdx, Model&& childObject, ShaderProgram shaderProgram,
	                                        float orbitRadius, float rotationSpeed, float bodyScale, float tiltDeg, bool needOrientationFix);

	// Temporary
protected:
	void initBuffers() override {}
	void deleteBuffers() override {}
	//
public:
	SolarSystem(const ShaderProgram& shaderProgram, unsigned int cylMinRes, unsigned int cylMaxRes, TMat modelMat = glm::mat4(1.0f));

	void animate();
	void draw() override;
};
