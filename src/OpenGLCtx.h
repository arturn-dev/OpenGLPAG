#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Object3D.h"
#include "SceneGraphNode.h"
#include "Camera.h"
#include "LightSource.h"
#include "Skybox.h"

class OpenGLCtx
{
	FPSCamera camera;
	
	// TODO: Currently shader programs are stored in two places: here and individual ones in every instance of the OpenGLRender class.
	// Let's try to remove them from here and retrieve them from the Models before the draw to set the uniforms
	// or pass some structure to the draw() method with information needed for the shaders' uniforms.
	std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;

	DirLight* dirLight;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	std::unique_ptr<Skybox> skybox;
	ShaderProgram skyboxShaderProgram;
	
	glm::mat4 projMat;
	bool wireframeMode = false;
	
	void renderInit(int windowW, int windowH);
	std::string getNthUniformName(const std::string& uniformName, unsigned i);

public:
	OpenGLCtx();

	void init();
	void render(int windowW, int windowH, 
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsStartIt,
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsEndIt);
	void render(int windowW, int windowH, Object3D* object);
	void render(int windowW, int windowH, SceneGraphNode* sceneGraphRoot, bool& wasDirty);
	void renderLights(int windowW, int windowH);

	const ShaderProgram* addShaderProgram(ShaderProgram&& shaderProgram);
	void addPointLight(PointLight* pointLight);
	void addSpotLight(SpotLight* spotLight);
	void setDirLight(DirLight* dirLight);
	void setSkybox(const std::string& texturesDirPath, ShaderProgram shaderProgram);
	void setWireframeMode(bool wireframeMode);
	DirLight* getDirLight();
	std::vector<PointLight*>& getPointLights();
	std::vector<SpotLight*>& getSpotLights();
	FPSCamera& getCamera();

	void deleteCtx();
};

