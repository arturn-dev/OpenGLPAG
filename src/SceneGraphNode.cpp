#include "SceneGraphNode.h"

#include <stdexcept>

void SceneGraphNode::updateModelMats(TMat parentModelMat, bool dirtyFlag)
{
	dirtyFlag |= localMat.dirtyFlag;
	if (dirtyFlag)
	{
		object->modelMat.setTMat(parentModelMat * localMat);
		localMat.dirtyFlag = false;
	}

	for (auto&& children : childrens)
	{
		children->updateModelMats(object->modelMat, dirtyFlag);
	}
}

bool SceneGraphNode::draw(TMat parentModelMat, bool dirtyFlag)
{
	if (object == nullptr)
		throw std::logic_error("Object bound to the scene graph's node can't be nullptr");
	
	dirtyFlag |= localMat.dirtyFlag;
	if (dirtyFlag)
	{
		object->modelMat.setTMat(parentModelMat * localMat);
		localMat.dirtyFlag = false;
	}

	if (isVisible)
		object->draw();

	bool wasDirty = dirtyFlag;	
	for (auto&& children : childrens)
	{
		wasDirty |= children->draw(object->modelMat, dirtyFlag);
	}

	return wasDirty;
}

SceneGraphNode::SceneGraphNode()
	: object(new NullObject())
{
}

SceneGraphNode::SceneGraphNode(std::unique_ptr<Object3D> object)
	: object(std::move(object)), localMat(object->modelMat)
{
}

SceneGraphNode* SceneGraphNode::attachChildren()
{
	childrens.emplace_back(std::make_unique<SceneGraphNode>());

	return (childrens.end() - 1)->get();
}

SceneGraphNode* SceneGraphNode::attachChildren(std::unique_ptr<SceneGraphNode> childNode)
{
	childrens.push_back(std::move(childNode));

	return (childrens.end() - 1)->get();
}

void SceneGraphNode::updateModelMats()
{
	updateModelMats(TMat(), localMat.dirtyFlag);
}

bool SceneGraphNode::draw()
{
	return draw(TMat(), localMat.dirtyFlag);
}

std::vector<std::unique_ptr<SceneGraphNode>>& SceneGraphNode::getChildrens()
{
	return childrens;
}

Object3D* SceneGraphNode::getObject()
{
	return object.get();
}
