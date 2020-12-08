#include "OpenGLCtx.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


OpenGLCtx::OpenGLCtx()
	: projMat(glm::mat4(1.0f)), camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{	
	
}

void OpenGLCtx::init()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	camera.moveFB(-10.0f);
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

	float ar = static_cast<float>(windowW) / static_cast<float>(windowH);
	projMat = glm::perspective(glm::radians(45.0f), ar, 0.01f, 10000.0f);
	//projMat = glm::ortho(-ar, ar, -1.0f, 1.0f, 0.01f, 10000.0f);

	glm::mat4 viewMat = camera.getViewMat();
	for (auto&& shaderProgram : shaderPrograms)
	{
		// Initialize matrices
		
		shaderProgram->setUniformMat4("view", viewMat);
		shaderProgram->setUniformVec3("viewPos", camera.getPosition());
		shaderProgram->setUniformMat4("proj", projMat);

		// Initialize lights

		if (dirLight != nullptr)
		{
			glm::vec3 color = dirLight->getColor();
			if (!dirLight->isLit())
				color = glm::vec3(0.0f);
			shaderProgram->setUniformVec3("dirLight.direction", dirLight->getDirection());
			shaderProgram->setUniformVec3("dirLight.lightColors.diffuse", color);
			shaderProgram->setUniformVec3("dirLight.lightColors.specular", color);
		}		
		
		for (auto&& pointLight : pointLights)
		{
			static int i = 0;

			glm::vec3 color = pointLight->getColor();
			if (!pointLight->isLit())
				color = glm::vec3(0.0f);
			
			shaderProgram->setUniformVec3(getNthUniformName("pointLights.position", i), pointLight->modelMat.getTMat()[3]);
			shaderProgram->setUniformVec3(getNthUniformName("pointLights.lightColors.diffuse", i), color);
			shaderProgram->setUniformVec3(getNthUniformName("pointLights.lightColors.specular", i), color);
			
			i++;
		}

		for (auto&& spotLight : spotLights)
		{
			static int i = 0;

			glm::vec3 color = spotLight->getColor();
			if (!spotLight->isLit())
				color = glm::vec3(0.0f);
			
			shaderProgram->setUniformVec3(getNthUniformName("spotLights.position", i), spotLight->modelMat.getTMat()[3]);
			shaderProgram->setUniformVec3(getNthUniformName("spotLights.direction", i), spotLight->getDirection());
			shaderProgram->setUniformFloat(getNthUniformName("spotLights.cutOffDeg", i), spotLight->getCutOffDeg());
			shaderProgram->setUniformVec3(getNthUniformName("spotLights.lightColors.diffuse", i), color);
			shaderProgram->setUniformVec3(getNthUniformName("spotLights.lightColors.specular", i), color);

			i++;
		}
	}	
}

std::string OpenGLCtx::getNthUniformName(const std::string& uniformName, unsigned i)
{
	std::string name = uniformName;
	
	auto dotIdx = name.find('.');
	if (dotIdx != std::string::npos)
	{
		name.insert(dotIdx, "[" + std::to_string(i) + "]");
	}
	else
	{
		name += std::to_string(i);
	}

	return name;
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

void OpenGLCtx::renderLights(int windowW, int windowH)
{
	if (dirLight != nullptr)
		dirLight->draw();
	
	for (auto&& pointLight : pointLights)
	{
		pointLight->draw();
	}

	for (auto&& spotLight : spotLights)
	{
		spotLight->draw();
	}
}

FPSCamera& OpenGLCtx::getCamera()
{
	return camera;
}

const ShaderProgram* OpenGLCtx::addShaderProgram(ShaderProgram&& shaderProgram)
{
	// TODO: Move setting the attributes to the ShaderProgram class itself.
	shaderProgram.setAttribPosByName("pos_in");
	shaderProgram.setAttribColByName("col_in");
	shaderProgram.setAttribTexByName("tex_in");
	shaderProgram.setAttribNormByName("norm_in");
	
	shaderPrograms.push_back(std::make_unique<ShaderProgram>(std::move(shaderProgram)));

	return (shaderPrograms.end() - 1)->get();
}

PointLight* OpenGLCtx::addPointLight(PointLight&& pointLight)
{
	pointLights.push_back(std::make_unique<PointLight>(std::move(pointLight)));

	return (pointLights.end() - 1)->get();
}

SpotLight* OpenGLCtx::addSpotLight(SpotLight&& spotLight)
{
	spotLights.push_back(std::make_unique<SpotLight>(std::move(spotLight)));

	return (spotLights.end() - 1)->get();
}

void OpenGLCtx::setWireframeMode(bool wireframeMode)
{
	this->wireframeMode = wireframeMode;
}

void OpenGLCtx::setDirLight(DirLight&& dirLight)
{
	this->dirLight = std::make_unique<DirLight>(std::move(dirLight));
}

DirLight* OpenGLCtx::getDirLight()
{
	return dirLight.get();
}

void OpenGLCtx::deleteCtx()
{
	for (auto&& shaderProgram : shaderPrograms)
	{
		shaderProgram->deleteProgram();
	}	
}
