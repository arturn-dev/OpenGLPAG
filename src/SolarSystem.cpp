#include "SolarSystem.h"

#include "AssimpModelLoader.h"
#include "Cuboid.h"

void SolarSystem::initElements()
{
	// TODO: vector of Celestial Bodies processing (setting objects' matrices from info in the structure)

	rootNode.localMat.setTMat(modelMat);
	
	int i = 0;
	for (auto&& element : solarSystemElements)
	{
		switch (element.type)
		{
			case SolarSystemElement::ElementOrbit:
			{
				element.objectNode->localMat.translate(glm::vec3(element.orbitRadius, 0.0f, 0.0f));
			}
			break;
			case SolarSystemElement::ElementBody:
			{
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
	
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody, 
									 rootNode.attachChildren(
										NODE_FROM_MODEL(modelLoader.loadModel("sun.obj"))), 
								     0.0f, 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	auto planet1OrbitNode = std::make_unique<SceneGraphNode>();
	auto moon1OrbitNode = std::make_unique<SceneGraphNode>();
	
	orbitRadius = 20.0f;
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 planet1OrbitNode->attachChildren(
										NODE_FROM_MODEL(modelLoader.loadModel("earth.obj"))), 
								     orbitRadius, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 moon1OrbitNode->attachChildren(
										std::make_unique<SceneGraphNode>(std::unique_ptr<Object3D>(ColCuboid::createCube(shaderProgram.getUniformLocation("model"), shaderProgram.getAttribPos(), shaderProgram.getAttribCol(), 1.0f)))), 
								     orbitRadius/5, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));

	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 planet1OrbitNode->attachChildren(std::move(moon1OrbitNode)),
									 orbitRadius/5);
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
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
			case SolarSystemElement::ElementOrbit:
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
				element.objectNode->localMat.rotate(selfAxisRotationSpeedFactor / element.bodyScale, element.rotAxis);
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
