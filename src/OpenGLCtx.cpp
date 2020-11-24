#include "OpenGLCtx.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void OpenGLCtx::prepareShaders()
{
    Shader vertexShader(".\\res\\shaders\\basic.vert", GL_VERTEX_SHADER);
	Shader fragmentShader(".\\res\\shaders\\basic.frag", GL_FRAGMENT_SHADER);
	vertexShader.compileShader();
	fragmentShader.compileShader();

    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
	shaderProgram.makeProgram();
}

OpenGLCtx::OpenGLCtx()
	: viewMat(glm::mat4(1.0f)), projMat(glm::mat4(1.0f))
{	
	
}

void OpenGLCtx::init()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	prepareShaders();
	shaderProgram.use();

	viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -10.0f));
	viewMat = glm::rotate(viewMat, glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 0.0f));	
	
	shaderProgram.setAttribPosByName("pos_in");
	shaderProgram.setAttribColByName("col_in");
	shaderProgram.setAttribTexByName("tex_in");
	shaderProgram.setAttribNormByName("norm_in");
}

void OpenGLCtx::renderInit(int windowW, int windowH)
{
	glViewport(0, 0, windowW, windowH);
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	float ar = static_cast<float>(windowW) / static_cast<float>(windowH);
	projMat = glm::perspective(glm::radians(45.0f), ar, 0.01f, 10000.0f);
	//projMat = glm::ortho(-ar, ar, -1.0f, 1.0f, 0.01f, 10000.0f);
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgramId(), "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgramId(), "proj"), 1, GL_FALSE, glm::value_ptr(projMat));
}

void OpenGLCtx::render(int windowW, int windowH, 
                       const std::vector<std::unique_ptr<Object3D>>::iterator objectsStartIt,
                       const std::vector<std::unique_ptr<Object3D>>::iterator objectsEndIt)
{
	renderInit(windowW, windowH);
	
	for (auto it = objectsStartIt; it < objectsEndIt; ++it)
	{
		it->get()->draw();
	}

	glBindVertexArray(0);
}

void OpenGLCtx::render(int windowW, int windowH, Object3D* object)
{
	renderInit(windowW, windowH);
	
	object->draw();
}

void OpenGLCtx::render(int windowW, int windowH, SceneGraphNode* sceneGraphRoot)
{
	renderInit(windowW, windowH);

	sceneGraphRoot->draw();
}

GLint OpenGLCtx::getModelMatLocation() const
{
	return glGetUniformLocation(shaderProgram.getProgramId(), "model");
}

GLuint OpenGLCtx::getAPos() const
{
	return aPos;
}

GLuint OpenGLCtx::getACol() const
{
	return aCol;
}

GLuint OpenGLCtx::getATex() const
{
	return aTex;
}

glm::mat4 OpenGLCtx::getViewMat() const
{
	return viewMat;
}

void OpenGLCtx::setViewMat(const glm::mat4& viewMat)
{
	this->viewMat = viewMat;
}

void OpenGLCtx::setWireframeMode(bool wireframeMode)
{
	this->wireframeMode = wireframeMode;
}

const ShaderProgram& OpenGLCtx::getShaderProgram() const
{
	return shaderProgram;
}

void OpenGLCtx::deleteCtx()
{
	shaderProgram.deleteProgram();
}
