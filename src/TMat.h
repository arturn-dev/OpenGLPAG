#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TMat
{
	glm::mat4 tMat;
	
public:
	bool dirtyFlag = false;

	TMat() : tMat(1.0f) {}
	TMat(const TMat& other) : tMat(other.tMat) {}
	TMat(TMat&& other) noexcept : tMat(other.tMat) {}
	TMat(const glm::mat4 mat) : tMat(mat) {}

	TMat& operator=(const TMat& other)
	{
		tMat = other.tMat;
		return *this;
	}
	
	TMat& translate(glm::vec3 vec)
	{
		tMat = glm::translate(tMat, vec);
		dirtyFlag = true;
		return *this;
	}

	TMat& rotate(float angleRad, glm::vec3 axis)
	{
		tMat = glm::rotate(tMat, angleRad, axis);
		dirtyFlag = true;
		return *this;
	}

	TMat& scale(glm::vec3 vec)
	{
		tMat = glm::scale(tMat, vec);
		dirtyFlag = true;
		return *this;
	}

	TMat operator*(const TMat& other) const
	{
		return tMat * other.tMat;
	}

	TMat& setTMat(const TMat& tMat)
	{
		this->tMat = tMat.tMat;
		dirtyFlag = true;
		return *this;
	}
	
	glm::mat4 getTMat() const
	{
		return tMat;
	}
};