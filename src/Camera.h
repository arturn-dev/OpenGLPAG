#pragma once

#include <glm/glm.hpp>

#include "TMat.h"

class Camera
{
protected:
	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up);
	virtual ~Camera() = default;

	void setPosition(const glm::vec3& position);
	void setCenter(const glm::vec3& center);
	glm::vec3 getPosition() const;
	virtual glm::mat4 getViewMat() const;
};

class FPSCamera : public Camera
{
	float pitch = 0.0f, yaw = 0.0f;
	
public:
	FPSCamera();
	FPSCamera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up);
	virtual ~FPSCamera() = default;

	void moveFB(float units);
	void moveLR(float units);
	void moveUD(float units);
	void rotate(float pitchDeg, float yawDeg);

	glm::mat4 getViewMat() const override;
};