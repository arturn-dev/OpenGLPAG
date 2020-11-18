#include "SolarSystem.h"


#include "Cylinder.h"
#include "Orbit.h"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"

void SolarSystem::initElements()
{
	// TODO: vector of Celestial Bodies processing (setting objects' matrices from info in the structure)

	rootNode.localMat.setTMat(modelMat);
	
	int i = 0;
	for (auto&& element : solarSystemElements)
	{
		switch (element.type)
		{
			case SolarSystemElement::ElementOrbiting:
			{
				element.objectNode->localMat.translate(glm::vec3(element.orbitRadius, 0.0f, 0.0f));
			}
			break;
			case SolarSystemElement::ElementOrbit:
			{
				element.objectNode->localMat.setTMat(modelMat);
			}
			break;
			case SolarSystemElement::ElementBody:
			{
				if (element.needOrientationFix)
				{
					element.objectNode->localMat.rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}
					
				glm::vec4 rotAxis = modelMat.getTMat() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
				element.objectNode->localMat.rotate(glm::radians(element.tiltDeg), glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
					
				element.objectNode->localMat.scale(glm::vec3(element.bodyScale));
			}
		}
		
		i++;
	}
}

template <typename T>
unsigned long long SolarSystem::addObjectToParent(int parentAttachNodeIdx, T&& childObject, ShaderProgram shaderProgram,
                                                     float orbitRadius, float rotationSpeed, float bodyScale, float tiltDeg, bool needOrientationFix, glm::vec4 color)
{
	SceneGraphNode* parentsNode;
	if (parentAttachNodeIdx == -1)
	{
		parentsNode = &rootNode;
	}		
	else
	{
		parentsNode = (solarSystemElements.begin() + parentAttachNodeIdx)->objectNode;
	}	

	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 parentsNode->attachChildren(NODE_FROM_MODEL(Orbit(shaderProgram, 500, orbitRadius, color))));
	
	auto orbiting = std::make_unique<SceneGraphNode>();
	
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 orbiting->attachChildren(NODE_FROM_MODEL(std::move(childObject))),
									 orbitRadius, rotationSpeed, bodyScale, tiltDeg, needOrientationFix);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbiting,
									 parentsNode->attachChildren(std::move(orbiting)), orbitRadius);

	auto attachNodeForOtherChildsIdx = solarSystemElements.size() - 1;

	return attachNodeForOtherChildsIdx;
}

unsigned long long SolarSystem::addTexModelToParent(int parentAttachNodeIdx, Model&& childObject,
	ShaderProgram shaderProgram, float orbitRadius, float rotationSpeed, float bodyScale, float tiltDeg,
	bool needOrientationFix)
{
	SceneGraphNode* parentsNode;
	if (parentAttachNodeIdx == -1)
	{
		parentsNode = &rootNode;
	}		
	else
	{
		parentsNode = (solarSystemElements.begin() + parentAttachNodeIdx)->objectNode;
	}	

	OpenGLRender::Texture objectTex = childObject.getMeshes()[0].getOpenGLRender().getTextures()[0];
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 parentsNode->attachChildren(NODE_FROM_MODEL(Orbit(shaderProgram, 500, orbitRadius, objectTex))));
	
	auto orbiting = std::make_unique<SceneGraphNode>();
	
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 orbiting->attachChildren(NODE_FROM_MODEL(std::move(childObject))),
									 orbitRadius, rotationSpeed, bodyScale, tiltDeg, needOrientationFix);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbiting,
									 parentsNode->attachChildren(std::move(orbiting)), orbitRadius);

	auto attachNodeForOtherChildsIdx = solarSystemElements.size() - 1;

	return attachNodeForOtherChildsIdx;
}

SolarSystem::SolarSystem(const ShaderProgram& shaderProgram, unsigned int cylMinRes, unsigned int cylMaxRes, TMat modelMat)
	: Object3D(modelMat), cylMinRes(cylMinRes), cylMaxRes(cylMaxRes)
{
	// TODO: Get textures for orbits i.e. new method in AssimpModelLoader for retrieving texture for given model.


	float orbitRs[] = {20.0f, 40.0f};
	
	AssimpModelLoader modelLoader(".\\res\\models", ".\\res\\textures", shaderProgram);

	/*Model earth = modelLoader.loadModel("earth.obj");
	OpenGLRender::Texture earthTex = earth.getMeshes()[0].getOpenGLRender().getTextures()[0];
	
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody, 
									 rootNode.attachChildren(
										NODE_FROM_MODEL(modelLoader.loadModel("sun.obj"))), 
								     orbitRs[0], 10.0f, 0, true);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 rootNode.attachChildren(NODE_FROM_MODEL(Orbit(shaderProgram, 100, 20.0f, earthTex))),
									 orbitRs[1]);	
	
	auto planet1OrbitNode = std::make_unique<SceneGraphNode>();
	auto moon1OrbitNode = std::make_unique<SceneGraphNode>();
	auto cylinderOrbitNode = std::make_unique<SceneGraphNode>();

	cylindersNodesIdx = solarSystemElements.size();
	for (int i = cylMinRes; i <= cylMaxRes; ++i)
	{
		Cylinder cylinder(i, 5.0f, 3.0f, shaderProgram);
		solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
										 rootNode.attachChildren(
											NODE_FROM_MODEL(std::move(cylinder))),
										 )
	}	
	orbitRadius = 20.0f;
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 planet1OrbitNode->attachChildren(
										NODE_FROM_MODEL(std::move(earth))), 
								     orbitRadius, 1.0f, 0);
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 moon1OrbitNode->attachChildren(
										NODE_FROM_MODEL(Cylinder(10, 10.0f, 2.0f, shaderProgram))), 
								     orbitRadius/5, 0.5f, 0);

	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbiting,
									 planet1OrbitNode->attachChildren(std::move(moon1OrbitNode)),
									 orbitRadius/5);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbiting,
									 rootNode.attachChildren(std::move(planet1OrbitNode)),
									 orbitRadius);*/

	/*auto sunAsParentIdx = addObjectToParent(-1, modelLoader.loadModel("sun.obj"), shaderProgram,
						 0.0f, 5.0f, 0.0f, true);*/

	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 rootNode.attachChildren(NODE_FROM_MODEL(modelLoader.loadModel("sun.obj"))),
									 0.0f, 0.05f, 5.0f, 0.0f, true);
	
	addTexModelToParent(-1, modelLoader.loadModel("earth.obj"), shaderProgram,
	                     orbitRs[0], 20.0f, 1.0f, 22.5f, false);

	glm::vec4 cylinderCol(1.0f, 0.0f, 0.0f, 1.0f);
	
	addObjectToParent(-1, Cylinder(10, 5.0f, 2.0f, shaderProgram, cylinderCol), shaderProgram,
					  orbitRs[1], 2.0f, 1.0f, 0.0f, false, cylinderCol);

	initElements();
}

void SolarSystem::animate()
{
	for (auto&& element : solarSystemElements)
	{
		switch (element.type)
		{
			case SolarSystemElement::ElementOrbiting:
			{
				element.objectNode->localMat.setTMat(
					glm::rotate(glm::mat4(1.0f), 
								orbitalSpeedFactor / element.orbitRadius, 
								glm::vec3(0.0f, 0.0f, 1.0f)) 
					* element.objectNode->localMat.getTMat());
			}
			break;
			case SolarSystemElement::ElementBody:
			{
				if (element.needOrientationFix)
				{
					element.objectNode->localMat.rotate(element.rotationSpeed, glm::vec3(0.0f, -1.0f, 0.0f));
				}
				else
				{
					element.objectNode->localMat.rotate(element.rotationSpeed, glm::vec3(0.0f, 0.0f, -1.0f));
				}				
			}
		}		
	}
}

void SolarSystem::draw()
{
	if (modelMat.dirtyFlag)
	{
		rootNode.localMat.setTMat(modelMat);
		modelMat.dirtyFlag = false;
	}
	
	rootNode.draw();
}
