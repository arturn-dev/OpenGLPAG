#include "SolarSystem.h"
#include "Cylinder.h"
#include "Orbit.h"

void SolarSystem::initElements()
{
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
					element.objectNode->localMat.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}
					
				glm::vec4 rotAxis = modelMat.getTMat() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

				element.objectNode->localMat.setTMat(
				glm::rotate(glm::mat4(1.0f), 
							glm::radians(element.tiltDeg), 
							glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z)) 
				* element.objectNode->localMat.getTMat());
					
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

void SolarSystem::prepareCylinders(ShaderProgram shaderProgram, float orbitRadius)
{
	glm::vec4 cylinderCol(1.0f, 0.0f, 0.0f, 1.0f);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 rootNode.attachChildren(NODE_FROM_MODEL(Orbit(shaderProgram, 500, orbitRadius, cylinderCol))));
	auto cylinderOrbitingNode = std::make_unique<SceneGraphNode>();
	cylindersNodesIdx = solarSystemElements.size();
	for (auto i = cylMinRes; i <= cylMaxRes; ++i)
	{
		solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
										 cylinderOrbitingNode->attachChildren(NODE_FROM_MODEL(Cylinder(i, 5.0f, 2.0f, shaderProgram, cylinderCol))),
										 orbitRadius, 0.1f, 1.0f, 0.0f, false);
		(solarSystemElements.end() - 1)->objectNode->isVisible = false;
	}
	solarSystemElements[cylindersNodesIdx].objectNode->isVisible = true;
	visibleCylIdx = cylindersNodesIdx;
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbiting,
									 rootNode.attachChildren(std::move(cylinderOrbitingNode)),
									 orbitRadius);
}

SolarSystem::SolarSystem(const ShaderProgram& shaderProgram, unsigned int cylMinRes, unsigned int cylMaxRes, TMat modelMat)
	: Object3D(modelMat), cylMinRes(cylMinRes), cylMaxRes(cylMaxRes)
{
	float planetOrbitsRs[] = {15.0f, 25.0f, 50.0f, 80.0f, 130.0f, 160.0f, 200.0f, 230.0f, 260.0f};
	float moonOrbitsRs[] = {5.0f, 5.0f, 8.0f, 10.0f, 7.0f, 11.0f, 13.0f, 18.0f};
	
	AssimpModelLoader modelLoader(".\\res\\models", ".\\res\\textures", shaderProgram);

	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 rootNode.attachChildren(NODE_FROM_MODEL(modelLoader.loadModel("sun.obj"))),
									 0.0f, 0.05f, 5.0f, 0.0f, true);
	
	addTexModelToParent(-1, modelLoader.loadModel("mercury.obj"), shaderProgram,
	                    planetOrbitsRs[0], 1.0f, 0.5f, 0.0f, true);
	addTexModelToParent(-1, modelLoader.loadModel("venus.obj"), shaderProgram,
	                    planetOrbitsRs[1], 1.0f, 0.7f, 2.0f, true);
	auto earthAttachNodeIdx = addTexModelToParent(-1, modelLoader.loadModel("earth.obj"), shaderProgram,
	                    planetOrbitsRs[2], 1.0f, 1.0f, 22.5f, true);
	addTexModelToParent(-1, modelLoader.loadModel("mars.obj"), shaderProgram, 
						planetOrbitsRs[3], 1.0f, 1.0f, 20.0f, true);
	auto jupiterAttachNodeIdx = addTexModelToParent(-1, modelLoader.loadModel("jupiter.obj"), shaderProgram,
	                    planetOrbitsRs[4], 1.0f, 2.0f, 1.0f, true);
	auto saturnAttachNodeIdx = addTexModelToParent(-1, modelLoader.loadModel("saturn.obj"), shaderProgram,
	                    planetOrbitsRs[5], 1.0f, 0.4f, 15.0f, true);
	addTexModelToParent(-1, modelLoader.loadModel("uranus.obj"), shaderProgram,
	                    planetOrbitsRs[6], 1.0f, 1.5f, 60.0f, true);
	addTexModelToParent(-1, modelLoader.loadModel("neptune.obj"), shaderProgram,
	                    planetOrbitsRs[7], 1.0f, 1.5f, 3.0f, true);
	prepareCylinders(shaderProgram, planetOrbitsRs[8]);

	addTexModelToParent(earthAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[0], 1.0f, 0.5f, 3.5f, true);
	addTexModelToParent(jupiterAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[1], 1.0f, 0.5f, 5.0f, true);
	addTexModelToParent(jupiterAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[2], 1.0f, 0.7f, 6.0f, true);
	addTexModelToParent(jupiterAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[3], 1.0f, 0.7f, 1.1f, true);
	addTexModelToParent(saturnAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[4], 1.0f, 0.3f, 1.3f, true);
	addTexModelToParent(saturnAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[5], 1.0f, 0.9f, 7.5f, true);
	addTexModelToParent(saturnAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[6], 1.0f, 0.3f, 43.0f, true);
	addTexModelToParent(saturnAttachNodeIdx, modelLoader.loadModel("moon.obj"), shaderProgram,
						moonOrbitsRs[7], 1.0f, 0.5f, 10.0f, true);
	
	initElements();
}

void SolarSystem::setCylinderRes(unsigned res)
{
	if (res >= cylMinRes && res <= cylMaxRes)
	{
		solarSystemElements[visibleCylIdx].objectNode->isVisible = false;
		visibleCylIdx = cylindersNodesIdx + res - cylMinRes;
		solarSystemElements[visibleCylIdx].objectNode->isVisible = true;
		
	}
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
					element.objectNode->localMat.rotate(element.rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					element.objectNode->localMat.rotate(element.rotationSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
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
