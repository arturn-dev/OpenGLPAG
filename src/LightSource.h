#pragma once

#include "Model.h"
#include "OpenGLRender.h"

class LightSource : public Object3D
{
	ShaderProgram shaderProgram;
	
	glm::vec3 color = glm::vec3(0.0f);
	bool lit = true;
	bool objectVisible = true;
	
	void setupShader();

protected:
	std::unique_ptr<Model<Mesh<PosVert>>> model;
	
	void attachModel(std::unique_ptr<Model<Mesh<PosVert>>> model);
	
public:
	LightSource();
	LightSource(const glm::vec3& color, ShaderProgram shaderProgram);
	virtual ~LightSource();

	LightSource(const LightSource& other);
	LightSource(LightSource&& other) noexcept;
	LightSource& operator=(LightSource other);
	friend void swap(LightSource& lhs, LightSource& rhs) noexcept
	{
		using std::swap;
		swap(lhs.modelMat, lhs.modelMat);
		swap(lhs.shaderProgram, rhs.shaderProgram);
		swap(lhs.model, rhs.model);
		swap(lhs.color, rhs.color);
	}

	Model<Mesh<PosVert>>* getModel();
	glm::vec3 getColor() const;
	void setColor(const glm::vec3& color);
	bool isLit() const;

	void changeState(bool lit, bool objectVisible);
	void changeLitStatus(bool lit);
	void changeObjectVisibility(bool visible);
	void draw() override;
};

class DirLight : public LightSource
{
	const glm::vec3 baseDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 rotationMat = glm::mat4(1.0f);
	
public:
	DirLight();
	DirLight(const glm::vec3& color, ShaderProgram shaderProgram);
	DirLight(const DirLight& other);
	DirLight(DirLight&& other) noexcept;
	DirLight& operator=(DirLight other);
	
	friend void swap(DirLight& lhs, DirLight& rhs) noexcept
	{
		using std::swap;
		swap(static_cast<LightSource&>(lhs), static_cast<LightSource&>(rhs));
	}

	void draw() override;

	void setDirection(const glm::vec3& direction);
	glm::vec3 getDirection() const;
};

class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(const glm::vec3& color, ShaderProgram shaderProgram);
	PointLight(const PointLight& other);
	PointLight(PointLight&& other) noexcept;
	PointLight& operator=(PointLight other);

	friend void swap(PointLight& lhs, PointLight& rhs) noexcept
	{
		using std::swap;
		swap(static_cast<LightSource&>(lhs), static_cast<LightSource&>(rhs));
	}

	void draw() override;
};

class SpotLight : public LightSource
{
	const glm::vec3 baseDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 rotationMat = glm::mat4(1.0f);
	float cutOffDeg = 0.0f;
	
public:
	SpotLight();
	SpotLight(const glm::vec3& color, const ShaderProgram& shaderProgram, float cutOffDeg);
	SpotLight(const SpotLight& other);
	SpotLight(SpotLight&& other) noexcept;
	SpotLight& operator=(SpotLight other);

	friend void swap(SpotLight& lhs, SpotLight& rhs) noexcept
	{
		using std::swap;
		swap(static_cast<LightSource&>(lhs), static_cast<LightSource&>(rhs));
		swap(lhs.cutOffDeg, rhs.cutOffDeg);
	}

	void draw() override;


	void setDirection(const glm::vec3& direction);
	glm::vec3 getDirection() const;
	float getCutOffDeg() const;
	void setCutOffDeg(const float cutOffDeg);
};