#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Object3D.h"
#include "SceneGraphNode.h"
#include "Camera.h"

class OpenGLCtx
{
	FPSCamera camera;
	// TODO: Currently shader programs are stored in two places: here and individual ones in every instance of the OpenGLRender class.
	// Let's try to remove them from here and retrieve them from the Models before the draw to set the uniforms
	// or pass some structure to the draw() method with information needed for the shaders' uniforms.
	std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
	glm::mat4 projMat;
	GLuint aPos, aCol, aTex;	
	bool wireframeMode = false;
	
	void renderInit(int windowW, int windowH);

public:
	OpenGLCtx();

	void init();
	void render(int windowW, int windowH, 
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsStartIt,
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsEndIt);
	void render(int windowW, int windowH, Object3D* object);
	void render(int windowW, int windowH, SceneGraphNode* sceneGraphRoot);

	GLuint getAPos() const;
	GLuint getACol() const;
	GLuint getATex() const;
	FPSCamera& getCamera();
	
	const ShaderProgram* addShaderProgram(ShaderProgram&& shaderProgram);
	void setWireframeMode(bool wireframeMode);

	void deleteCtx();
};

