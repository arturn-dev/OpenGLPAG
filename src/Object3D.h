#pragma once

#include "TMat.h"


class Object3D
{
public:
	TMat modelMat;

	Object3D() {}
	explicit Object3D(const TMat& modelMat) : modelMat(modelMat) {}
	virtual ~Object3D() {}

	virtual void draw() = 0;
};
