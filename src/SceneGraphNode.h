#pragma once

#include <memory>
#include <vector>

#include "TMat.h"
#include "Object3D.h"

class NullObject : public Object3D
{
public:
	void draw() override {}
};

class SceneGraphNode
{
public:
	TMat localMat; // Transformation matrix of the object related to the parent node's object.
	bool isVisible = true;

private:	
	std::unique_ptr<Object3D> object;
	std::vector<std::unique_ptr<SceneGraphNode>> childrens;

	void updateModelMats(TMat parentModelMat, bool dirtyFlag);
	bool draw(TMat parentModelMat, bool dirtyFlag);

public:
	SceneGraphNode();
	SceneGraphNode(std::unique_ptr<Object3D> object);

	SceneGraphNode* attachChildren();
	SceneGraphNode* attachChildren(std::unique_ptr<SceneGraphNode> childNode);

	void updateModelMats();
	bool draw();

	std::vector<std::unique_ptr<SceneGraphNode>>& getChildrens();
	Object3D* getObject();
};

template <typename T>
constexpr auto NODE_FROM_MODEL(const T& object3D)
{
	return std::make_unique<SceneGraphNode>(std::make_unique<T>(std::move(object3D)));
}