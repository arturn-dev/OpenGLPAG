#include "SolarSystem.h"

#include "AssimpModelLoader.h"
#include "Cylinder.h"
#include "Orbit.h"

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

SolarSystem::SolarSystem(const ShaderProgram& shaderProgram, TMat modelMat)
	: Object3D(modelMat)
{
	// TODO: Get textures for orbits i.e. new method in AssimpModelLoader for retrieving texture for given model.


	float orbitRadius = 0.0f;
	
	AssimpModelLoader modelLoader(".\\res\\models", ".\\res\\textures", shaderProgram);

	Model earth = modelLoader.loadModel("earth.obj");
	OpenGLRender::Texture earthTex = earth.getMeshes()[0].getOpenGLRender().getTextures()[0];
	
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody, 
									 rootNode.attachChildren(
										NODE_FROM_MODEL(modelLoader.loadModel("sun.obj"))), 
								     0.0f, 10.0f, 0, true);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 rootNode.attachChildren(NODE_FROM_MODEL(Orbit(shaderProgram, 100, 20.0f, earthTex))),
									 20.0f);

	auto planet1OrbitNode = std::make_unique<SceneGraphNode>();
	auto moon1OrbitNode = std::make_unique<SceneGraphNode>();
	
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
									 orbitRadius);

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
					element.objectNode->localMat.rotate(selfAxisRotationSpeedFactor / element.bodyScale, glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					element.objectNode->localMat.rotate(selfAxisRotationSpeedFactor / element.bodyScale, glm::vec3(0.0f, 0.0f, 1.0f));
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
