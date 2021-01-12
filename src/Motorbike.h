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

	float speed = 0.0f;

public:
	MotorbikeNode(const ShaderProgram* shaderProgram);
};
