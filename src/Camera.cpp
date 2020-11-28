#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
	: position(position), center(center), up(up)
{
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

FPSCamera::FPSCamera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
	: Camera(position, center, up)
{
}

void FPSCamera::moveFB(float units)
{
	position += center * units;
}

void FPSCamera::moveLR(float units)
{
	position -= glm::normalize(glm::cross(center, up)) * units;
}

void FPSCamera::rotate(float pitchDeg, float yawDeg)
{
	glm::vec3 vec;
	const auto maxPitch = glm::pi<float>() / 2 - 0.01f;
	pitch += glm::radians(pitchDeg);
	if (pitch > maxPitch)
		pitch = maxPitch;
	else if (pitch < -maxPitch)
		pitch = -maxPitch;

	const auto twoPi = glm::pi<float>() * 2;
	yaw += glm::radians(yawDeg);
	if (yaw > twoPi)
		yaw = yaw - twoPi;
	else if (yaw < -twoPi)
		yaw = yaw + twoPi;
	
	vec.x = glm::cos(pitch) * glm::cos(yaw);
	vec.y = glm::sin(pitch);
	vec.z = glm::cos(pitch) * glm::sin(yaw);

	center = glm::normalize(vec);
}

glm::mat4 FPSCamera::getViewMat() const
{
	return glm::lookAt(position, position + center, up);
}
