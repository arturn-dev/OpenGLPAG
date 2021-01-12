#include "Motorbike.h"
#include "AssimpModelLoader.h"

MotorbikeNode::MotorbikeNode(const ShaderProgram* shaderProgram)
{
	AssimpModelLoader<TexMesh> modelLoader(".\\res\\models", "");
	auto motorBody = modelLoader.loadModel("motor_body.obj", *shaderProgram);
	auto motorHandleBar = modelLoader.loadModel("motor_handle_bar.obj", *shaderProgram);
	auto motorWindshield= modelLoader.loadModel("motor_windshield.obj", *shaderProgram);
	auto motorFrontWheel= modelLoader.loadModel("motor_front_wheel.obj", *shaderProgram);
	auto motorBackWheel= modelLoader.loadModel("motor_back_wheel.obj", *shaderProgram);
	auto motorLeftMirror= modelLoader.loadModel("motor_left_mirror.obj", *shaderProgram);
	auto motorRightMirror= modelLoader.loadModel("motor_right_mirror.obj", *shaderProgram);

	motorBodyNode = attachChildren(NODE_FROM_MODEL(motorBody));
	motorHandleBarNode = motorBodyNode->attachChildren(NODE_FROM_MODEL(motorHandleBar));
	motorWindshieldNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorWindshield));
	motorFrontWheelNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorFrontWheel));
	motorBackWheelNode = motorBodyNode->attachChildren(NODE_FROM_MODEL(motorBackWheel));
	motorLeftMirrorNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorLeftMirror));
	motorRightMirrorNode = motorHandleBarNode->attachChildren(NODE_FROM_MODEL(motorRightMirror));
}
