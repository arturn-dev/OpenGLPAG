#include "SolarSystem.h"

#include "AssimpModelLoader.h"

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
				//element.objectNode.localMat.translate(glm::vec3(element.orbitRadius, 0.0f, 0.0f));
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
										std::make_unique<SceneGraphNode>(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								     0.0f, 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	std::unique_ptr<SceneGraphNode> planet1OrbitNode = std::make_unique<SceneGraphNode>();
	orbitRadius = 5.0f;
	solarSystemElements.emplace_back(SolarSystemElement::ElementBody,
									 planet1OrbitNode->attachChildren(
										std::make_unique<SceneGraphNode>(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								     orbitRadius, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	solarSystemElements.emplace_back(SolarSystemElement::ElementOrbit,
									 rootNode.attachChildren(std::move(planet1OrbitNode)),
									 orbitRadius);

	initElements();
}

void SolarSystem::animate()
{

	static float time = 0.0f;
	for (auto&& element : solarSystemElements)
	{
		switch (element.type)
		{
			case SolarSystemElement::ElementOrbit:
			{
				glm::vec4 rotAxis = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // TODO: Why 'z' axis and not 'y'?
				rotAxis = modelMat.getTMat() * rotAxis;
				element.objectNode->localMat.setTMat(TMat());
				element.objectNode->localMat.rotate(time, glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
				element.objectNode->localMat.translate(glm::vec3(element.orbitRadius, 0.0f, 0.0f));
			}
			break;
			case SolarSystemElement::ElementBody:
			{
				element.objectNode->localMat.rotate(1.0f / 10, element.rotAxis);
			}
		}		
	}

	time += 0.01f;
}

void SolarSystem::draw()
{
	rootNode.draw();
}
