#include "Motorbike.h"
#include "AssimpModelLoader.h"

MotorbikeNode::MotorbikeNode(const ShaderProgram* mainShader, const ShaderProgram* reflectiveShader, const ShaderProgram* refractiveShader, const std::string& skyboxTexturesDir)
{
	AssimpModelLoader<TexMesh> modelLoader(".\\res\\models", "");
	AssimpModelLoader<Mesh<PosVert>> modelLoader2(".\\res\\models", "");
	auto motorBody = modelLoader.loadModel("motor_body.obj", *mainShader);
	auto motorHandleBar = modelLoader.loadModel("motor_handle_bar.obj", *mainShader);
	auto motorWindshield= modelLoader2.loadModel("motor_windshield.obj", *refractiveShader);
	auto motorFrontWheel= modelLoader.loadModel("motor_front_wheel.obj", *mainShader);
	auto motorBackWheel= modelLoader.loadModel("motor_back_wheel.obj", *mainShader);
	auto motorLeftMirror= modelLoader2.loadModel("motor_left_mirror.obj", *reflectiveShader);
	auto motorRightMirror= modelLoader2.loadModel("motor_right_mirror.obj", *reflectiveShader);

	for (auto&& mesh : motorWindshield.getMeshes())
	{
		mesh.getOpenGLRender().addTexture({OpenGLRender::Texture::TexCubemap, skyboxTexturesDir});
	}
	for (auto&& mesh : motorLeftMirror.getMeshes())
	{
		mesh.getOpenGLRender().addTexture({OpenGLRender::Texture::TexCubemap, skyboxTexturesDir});
	}
	for (auto&& mesh : motorRightMirror.getMeshes())
	{
		mesh.getOpenGLRender().addTexture({OpenGLRender::Texture::TexCubemap, skyboxTexturesDir});
	}

	motorBodyNode = attachChildren(NODE_FROM_MODEL(motorBody));
	
	auto motorHandleBarTranslationNode = motorBodyNode->attachChildren();
	motorHandleBarTranslationNode->localMat.translate(glm::vec3(4.1362f, 0.80843f, 0.0f));
	motorHandleBarNode = motorHandleBarTranslationNode->attachChildren(NODE_FROM_MODEL(motorHandleBar));
	
	motorWindshieldNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorWindshield));
	motorWindshieldNode->localMat.translate(glm::vec3(-0.362164f, 1.44469f, 0.0f));

	auto motorFrontWheelTranslationNode = motorHandleBarNode->attachChildren();
	motorFrontWheelTranslationNode->localMat.translate(glm::vec3(0.836126f, -1.89739f, 0.0f));	
	motorFrontWheelNode = motorFrontWheelTranslationNode->attachChildren(NODE_FROM_MODEL(motorFrontWheel));

	auto motorBackWheelTranslationNode = motorBodyNode->attachChildren();
	motorBackWheelTranslationNode->localMat.translate(glm::vec3(0.0f, -1.05887f, 0.0f));
	motorBackWheelNode = motorBackWheelTranslationNode->attachChildren(NODE_FROM_MODEL(motorBackWheel));
	
	motorLeftMirrorNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorLeftMirror));
	motorRightMirrorNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorRightMirror));

	direction = glm::vec3(1.0f, 0.0f, 0.0f);
	
	// Calculate rotation axis for handle bar (front wheel)
	const float angleDeg = 25.8f;
	glm::vec4 verticalVec(0.0f, 1.0f, 0.0f, 0.0f);
	auto rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.0f, 0.0f, 1.0f));

	frontWheelRotationAxis = glm::normalize(rotMat * verticalVec);
}

void MotorbikeNode::accelerate()
{
	speed += acceleration;
	if (speed > maxSpeed)
		speed = maxSpeed;

	wasSpeedChanged = true;
}

void MotorbikeNode::decelerate()
{
	speed -= deceleration;
	if (speed < 0)
		speed = 0.0f;
}

void MotorbikeNode::applyBreaks()
{
	speed -= breakSpeed;
	if (speed < 0)
		speed = 0.0f;

	wasSpeedChanged = true;
}

void MotorbikeNode::turnLeft()
{
	frontWheelAngleDeg += frontWheelAngleDegIncrement;
	if (frontWheelAngleDeg > maxFrontWheelAngleDeg)
		frontWheelAngleDeg = maxFrontWheelAngleDeg;	
}

void MotorbikeNode::turnRight()
{
	frontWheelAngleDeg -= frontWheelAngleDegIncrement;
	if (frontWheelAngleDeg < -maxFrontWheelAngleDeg)
		frontWheelAngleDeg = -maxFrontWheelAngleDeg;
}


void MotorbikeNode::animate()
{
	if (!wasSpeedChanged)
		decelerate();

	// Calculate rotation
	float angleDelta = 0.0f;
	
	if (glm::abs(frontWheelAngleDeg) > 0.01f)
	{
		const float frontWheelTrajectoryRadius = wheelsSpan / glm::cos(glm::radians(90.0f - glm::abs(frontWheelAngleDeg)));
		angleDelta = speed / frontWheelTrajectoryRadius;
		if (frontWheelAngleDeg < 0)
			angleDelta *= -1;

		glm::vec4 directionVec4(direction.x, direction.y, direction.z, 0.0f);
		auto rotMat = glm::rotate(glm::mat4(1.0f), angleDelta, glm::vec3(0.0f, 1.0f, 0.0f));
		direction = glm::normalize(rotMat * directionVec4);
	}
	
	localMat.translate(direction * speed);
	motorBodyNode->localMat.rotate(angleDelta, glm::vec3(0.0f, 1.0f, 0.0f));

	motorHandleBarNode->localMat.setTMat(TMat());
	motorHandleBarNode->localMat.rotate(glm::radians(frontWheelAngleDeg), frontWheelRotationAxis);

	motorFrontWheelNode->localMat.rotate(speed / frontWheelRadius, glm::vec3(0.0f, 0.0f, -1.0f));
	motorBackWheelNode->localMat.rotate(speed / backWheelRadius, glm::vec3(0.0f, 0.0f, -1.0f));

	wasSpeedChanged = false;
}

glm::vec3 MotorbikeNode::getDirection() const
{
	return direction;
}
