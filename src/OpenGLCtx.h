#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Object3D.h"
#include "Model.h"

class OpenGLCtx
{
	ShaderProgram shaderProgram;	
	glm::mat4 viewMat, projMat;
	GLuint aPos, aCol, aTex;
	
	void prepareShaders();

public:
	OpenGLCtx();

	void init();
	void render(int windowW, int windowH, 
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsStartIt,
				const std::vector<std::unique_ptr<Object3D>>::iterator objectsEndIt);
	void render(int windowW, int windowH, const Model* model);

	GLint getModelMatLocation() const;
	GLuint getAPos() const;
	GLuint getACol() const;
	GLuint getATex() const;
	glm::mat4 getViewMat() const;
	const ShaderProgram& getShaderProgram() const;

	void deleteCtx();
};

