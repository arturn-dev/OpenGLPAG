#include "SceneGraphNode.h"

#include <stdexcept>

void SceneGraphNode::draw(TMat parentModelMat, bool dirtyFlag)
{
	if (object == nullptr)
		throw std::logic_error("Object bound to the scene graph's node can't be nullptr");
	
	dirtyFlag |= localMat.dirtyFlag;
	if (dirtyFlag)
	{
		object->modelMat.setTMat(parentModelMat * localMat);
		localMat.dirtyFlag = false;
	}

	object->draw();

	for (auto&& children : childrens)
	{
		children->draw(object->modelMat, dirtyFlag);
	}
}

SceneGraphNode::SceneGraphNode()
	: object(new NullObject())
{
}

SceneGraphNode::SceneGraphNode(std::unique_ptr<Object3D> object)
	: object(std::move(object)), localMat(object->modelMat)
{
}

SceneGraphNode* SceneGraphNode::attachChildren(std::unique_ptr<SceneGraphNode> childNode)
{
	childrens.push_back(std::move(childNode));

	return (childrens.end() - 1)->get();
}

void SceneGraphNode::draw()
{
	draw(TMat(), localMat.dirtyFlag);
}
