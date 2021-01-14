#pragma once

#include "SceneGraphNode.h"

class ShaderProgram;

class MotorbikeNode : public SceneGraphNode
{
	SceneGraphNode* motorBodyNode; // Motorbike's root node
	SceneGraphNode* motorHandleBarNode;
	SceneGraphNode* motorWindshieldNode;
	SceneGraphNode* motorFrontWheelNode;
	SceneGraphNode* motorBackWheelNode;
	SceneGraphNode* motorLeftMirrorNode;
	SceneGraphNode* motorRightMirrorNode;

	bool wasSpeedChanged = false;

	float speed = 0.0f;
	float frontWheelAngleDeg = 0.0f;

	const float acceleration = 0.05f;
	const float deceleration = 0.005f;
	const float breakSpeed = 0.1f;
	const float maxSpeed = 1.0f;
	const float maxFrontWheelAngleDeg = 70.0f;
	const float frontWheelAngleDegIncrement = 2.0f;
	const float wheelsSpan = 4.97231f;
	const float frontWheelRadius = 2.3f / 2;
	const float backWheelRadius = 2.39f / 2;

	glm::vec3 frontWheelRotationAxis;
	glm::vec3 direction;

public:
	MotorbikeNode(const ShaderProgram* shaderProgram);
	~MotorbikeNode() = default;

	void accelerate();
	void decelerate();
	void applyBreaks();
	void turnLeft();
	void turnRight();

	void animate();
};
