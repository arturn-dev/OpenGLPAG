#include "SolarSystem.h"


#include "AssimpModelLoader.h"
#include "Model.h"

void SolarSystem::initCelestialBodies()
{
	// TODO: vector of Celestial Bodies processing (setting objects' matrices from info in the structure)
	int i = 0;
	for (auto&& body : celestialBodies)
	{
		if (i == 0)
		{
			body.objectNode.localMat.setTMat(modelMat);
		}

		
		
		i++;
	}
}

SolarSystem::SolarSystem(ShaderProgram shaderProgram, TMat modelMat)
	: Object3D(modelMat)
{
	AssimpModelLoader modelLoader(".\\res\\models", ".\\res\textures", shaderProgram);
	
	SceneGraphNode solarSysNode;
	celestialBodies.emplace_back(solarSysNode.attachChildren(
									SceneGraphNode(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	SceneGraphNode planet1OrbitNode;
	celestialBodies.emplace_back(planet1OrbitNode.attachChildren(
									SceneGraphNode(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								 5.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	solarSysNode.attachChildren(std::move(planet1OrbitNode));

	SceneGraphNode planet2OrbitNode;
	celestialBodies.emplace_back(planet2OrbitNode.attachChildren(
									SceneGraphNode(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								 10.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	solarSysNode.attachChildren(std::move(planet2OrbitNode));

	SceneGraphNode planet3OrbitNode;
	celestialBodies.emplace_back(planet3OrbitNode.attachChildren(
									SceneGraphNode(std::make_unique<Model>(modelLoader.loadModel("earth.obj")))), 
								 15.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	solarSysNode.attachChildren(std::move(planet3OrbitNode));

}
