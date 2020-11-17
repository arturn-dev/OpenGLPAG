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
	void draw() override {}
};

class SceneGraphNode
{
public:
	TMat localMat; // Transformation matrix of the object related to the parent node's object.

private:	
	std::unique_ptr<Object3D> object;
	std::vector<std::unique_ptr<SceneGraphNode>> childrens;

	void draw(TMat parentModelMat, bool dirtyFlag);

public:
	SceneGraphNode();
	SceneGraphNode(std::unique_ptr<Object3D> object);

	SceneGraphNode* attachChildren(std::unique_ptr<SceneGraphNode> childNode);
	void draw();
};

template <typename T>
constexpr auto NODE_FROM_MODEL(T&& object3D)
{
	return std::make_unique<SceneGraphNode>(std::make_unique<T>(std::move(object3D)));
}