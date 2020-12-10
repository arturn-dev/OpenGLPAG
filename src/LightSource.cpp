#include "LightSource.h"
#include "AssimpModelLoader.h"

LightSource::LightSource()
{
}

LightSource::LightSource(const glm::vec3& color, ShaderProgram shaderProgram)
	: color(color), shaderProgram(shaderProgram)
{
	
}

LightSource::~LightSource()
{
}

LightSource::LightSource(const LightSource& other)
		: Object3D(other),
		  shaderProgram(other.shaderProgram),
		  model(std::make_unique<Model<Mesh<PosVert>>>(*other.model)),
		  color(other.color)
{
}

LightSource::LightSource(LightSource&& other) noexcept
{
	swap(*this, other);
}

LightSource& LightSource::operator=(LightSource other)
{
	swap(*this, other);
	
	return *this;
}

Model<Mesh<PosVert>>* LightSource::getModel()
{
	return model.get();
}

glm::vec3 LightSource::getColor() const
{
	return color;
}

void LightSource::setColor(const glm::vec3& color)
{
	this->color = color;
}

bool LightSource::isLit() const
{
	return lit;
}

void LightSource::changeState(bool lit, bool objectVisible)
{
	changeLitStatus(lit);
	changeObjectVisibility(objectVisible);
}

void LightSource::changeLitStatus(bool lit)
{
	this->lit = lit;
}

void LightSource::changeObjectVisibility(bool visible)
{
	objectVisible = visible;
}

void LightSource::draw()
{
	if (model != nullptr && objectVisible)
	{
		setupShader();
		model->draw();
	}		
}

void LightSource::setupShader()
{
	glm::vec3 lightColor = color;
	if(!lit)
	{
		lightColor = color * glm::vec3(0.2f);
	}
	
	shaderProgram.setUniformVec3("lightColor", lightColor);
}

void LightSource::attachModel(std::unique_ptr<Model<Mesh<PosVert>>> model)
{
	this->model = std::move(model);
}

DirLight::DirLight()
{
}

DirLight::DirLight(const glm::vec3& color, ShaderProgram shaderProgram)
	: LightSource(color, shaderProgram)
{
	AssimpModelLoader<Mesh<PosVert>> modelLoader(".\\res\\models", ".\\res\\textures");
	auto model = std::make_unique<Model<Mesh<PosVert>>>(modelLoader.loadModel("cube.obj", shaderProgram, aiColor4D{1.0f}));
	attachModel(std::move(model));
}

DirLight::DirLight(const DirLight& other)
	: LightSource(other)
{
}

DirLight::DirLight(DirLight&& other) noexcept
{
	swap(*this, other);
}

DirLight& DirLight::operator=(DirLight other)
{
	swap(*this, other);
	
	return *this;
}

void DirLight::draw()
{
	model->modelMat = modelMat.getTMat() * rotationMat;
	LightSource::draw();
}

void DirLight::setDirection(const glm::vec3& direction)
{
	if (direction == baseDirection)
	{
		rotationMat = glm::mat4(1.0f);

		return;
	}

	if (direction == -baseDirection)
	{
		rotationMat = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

		return;
	}
	
	glm::vec3 directionNormalized = glm::normalize(direction);
	glm::vec3 rotAxis = glm::cross(baseDirection, directionNormalized);
	float angle = acos(glm::dot(baseDirection, directionNormalized));

	rotationMat = glm::rotate(glm::mat4(1.0f), angle, rotAxis);
}

glm::vec3 DirLight::getDirection() const
{
	return glm::normalize(glm::mat3(model->modelMat.getTMat()) * baseDirection);
}

PointLight::PointLight()
{
}

PointLight::PointLight(const glm::vec3& color, ShaderProgram shaderProgram)
	: LightSource(color, shaderProgram)
{
	AssimpModelLoader<Mesh<PosVert>> modelLoader(".\\res\\models", ".\\res\\textures");
	auto model = std::make_unique<Model<Mesh<PosVert>>>(modelLoader.loadModel("sphere.obj", shaderProgram, aiColor4D{1.0f}));
	attachModel(std::move(model));
}

PointLight::PointLight(const PointLight& other)
	: LightSource(other)
{
}

PointLight::PointLight(PointLight&& other) noexcept
{
	swap(*this, other);
}

PointLight& PointLight::operator=(PointLight other)
{
	swap(*this, other);

	return *this;
}

void PointLight::draw()
{
	model->modelMat = modelMat.getTMat();
	LightSource::draw();
}

SpotLight::SpotLight()
{
}

SpotLight::SpotLight(const glm::vec3& color, const ShaderProgram& shaderProgram, float cutOffDeg)
	: LightSource(color, shaderProgram), cutOffDeg(cutOffDeg)
{
	AssimpModelLoader<Mesh<PosVert>> modelLoader(".\\res\\models", ".\\res\\textures");
	auto model = std::make_unique<Model<Mesh<PosVert>>>(modelLoader.loadModel("cube.obj", shaderProgram, aiColor4D{1.0f}));
	attachModel(std::move(model));
}

SpotLight::SpotLight(const SpotLight& other)
	: LightSource(other), cutOffDeg(other.cutOffDeg)
{
}

SpotLight::SpotLight(SpotLight&& other) noexcept
{
	swap(*this, other);
}

SpotLight& SpotLight::operator=(SpotLight other)
{
	swap(*this, other);

	return *this;
}

void SpotLight::draw()
{
	model->modelMat = modelMat.getTMat() * rotationMat;
	LightSource::draw();
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	if (direction == baseDirection)
	{
		rotationMat = glm::mat4(1.0f);

		return;
	}

	if (direction == -baseDirection)
	{
		rotationMat = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

		return;
	}
	
	glm::vec3 directionNormalized = glm::normalize(direction);
	glm::vec3 rotAxis = glm::cross(baseDirection, directionNormalized);
	float angle = acos(glm::dot(baseDirection, directionNormalized));

	rotationMat = glm::rotate(glm::mat4(1.0f), angle, rotAxis);
}

glm::vec3 SpotLight::getDirection() const
{
	return glm::normalize(glm::mat3(model->modelMat.getTMat()) * baseDirection);
}

float SpotLight::getCutOffDeg() const
{
	return cutOffDeg;
}

void SpotLight::setCutOffDeg(const float cutOffDeg)
{
	this->cutOffDeg = cutOffDeg;
}
