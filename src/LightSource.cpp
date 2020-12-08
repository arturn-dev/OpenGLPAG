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

void LightSource::turnOff()
{
	lit = false;
}

void LightSource::turnOn()
{
	lit = true;
}

void LightSource::draw()
{
	if (model != nullptr)
	{
		model->modelMat.setTMat(modelMat);
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

void DirLight::rotateModelToDirection()
{
	const glm::vec3 baseVec{0.0f, 0.0f, -1.0f};
	float angle = glm::acos(glm::dot(baseVec, direction) / glm::length(direction));
	glm::vec3 rotVec = glm::cross(baseVec, direction);
	
	modelMat.setTMat(TMat());
	modelMat.rotate(angle, rotVec);
}

DirLight::DirLight()
{
}

DirLight::DirLight(const glm::vec3& direction, const glm::vec3& color, ShaderProgram shaderProgram)
	: LightSource(color, shaderProgram), direction(direction)
{
	AssimpModelLoader<Mesh<PosVert>> modelLoader(".\\res\\models", ".\\res\\textures");
	auto model = std::make_unique<Model<Mesh<PosVert>>>(modelLoader.loadModel("cube.obj", shaderProgram, aiColor4D{1.0f}));
	attachModel(std::move(model));
}

DirLight::DirLight(const DirLight& other)
	: LightSource(other), direction(other.direction)
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
	rotateModelToDirection();
	LightSource::draw();
}

glm::vec3 DirLight::getDirection() const
{
	return direction;
}

void DirLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
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
	LightSource::draw();
}

SpotLight::SpotLight()
{
}

SpotLight::SpotLight(const glm::vec3& color, const ShaderProgram& shaderProgram, const glm::vec3& direction,
                     float cutOffDeg)
	: LightSource(color, shaderProgram), direction(direction), cutOffDeg(cutOffDeg)
{
	AssimpModelLoader<Mesh<PosVert>> modelLoader(".\\res\\models", ".\\res\\textures");
	auto model = std::make_unique<Model<Mesh<PosVert>>>(modelLoader.loadModel("cube.obj", shaderProgram, aiColor4D{1.0f}));
	attachModel(std::move(model));
}

SpotLight::SpotLight(const SpotLight& other)
	: LightSource(other), direction(other.direction), cutOffDeg(other.cutOffDeg)
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
	LightSource::draw();
}

glm::vec3 SpotLight::getDirection() const
{
	return direction;
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

float SpotLight::getCutOffDeg() const
{
	return cutOffDeg;
}

void SpotLight::setCutOffDeg(const float cutOffDeg)
{
	this->cutOffDeg = cutOffDeg;
}
