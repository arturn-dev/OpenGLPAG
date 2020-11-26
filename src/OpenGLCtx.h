#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Object3D.h"
#include "SceneGraphNode.h"

class OpenGLCtx
{
	std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
	glm::mat4 viewMat, projMat;
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
	glm::mat4 getViewMat() const;

	const ShaderProgram* addShaderProgram(ShaderProgram&& shaderProgram);
	void setViewMat(const glm::mat4& viewMat);
	void setWireframeMode(bool wireframeMode);

	void deleteCtx();
};

