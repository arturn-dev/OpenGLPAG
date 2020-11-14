#pragma once

#include <memory>
#include <vector>

#include "TMat.h"
#include "Object3D.h"

class NullObject : public Object3D
{
protected:
	void initBuffers() override {}
	void deleteBuffers() override {}
public:
	void draw() const override {}
};

class SceneGraphNode
{
public:
	TMat localMat; // Transformation matrix of the object related to the parent node's object.

private:	
	std::unique_ptr<Object3D> object;
	std::vector<SceneGraphNode> childrens;

	void draw(TMat parentModelMat, bool dirtyFlag);

public:
	SceneGraphNode();
	SceneGraphNode(std::unique_ptr<Object3D> object);

	SceneGraphNode& attachChildren(SceneGraphNode&& childNode);
	void draw();
};