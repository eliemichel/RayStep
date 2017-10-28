#include "SceneGraph.h"

#include "Logger.h"

SceneTree::SceneTree(SceneTree *parent)
	: m_parent(parent)
	, m_name("<node>")
{

}

SceneTree::~SceneTree()
{
	while (!m_children.empty())
	{
		delete m_children.back();
		m_children.pop_back();
	}
}

void SceneTree::addChild(SceneTree *node, int beforeIndex)
{
	node->setParent(this);
	if (beforeIndex < 0 || beforeIndex >= m_children.size())
	{
		m_children.push_back(node);
	}
	else
	{
		m_children.insert(m_children.begin() + beforeIndex, node);
	}
}

void SceneTree::removeChild(int childIndex)
{
	delete m_children[childIndex];
	m_children.erase(m_children.begin() + childIndex);
}

SceneTree *SceneTree::takeChild(int childIndex)
{
	SceneTree * node = m_children[childIndex];
	m_children.erase(m_children.begin() + childIndex);
	node->setParent(nullptr);
	return node;
}

std::string SceneTree::compileToGlsl(std::string target) const
{
	if (isRoot())
	{
		// TODO: clean up
		if (m_children.empty())
		{
			ERR_LOG << "Root must have one child!";
			return "";
		}
		if (target.empty())
		{
			return m_children[0]->compileToGlsl();
		}
		else
		{
			return m_children[0]->compileToGlsl("content") + "\n" + target + " = opU(" + target + ", content);";
		}
	}
	else
	{
		ERR_LOG << "Not implemented";
		return "";
	}
}

std::string SceneOperationNode::compileToGlsl(std::string target) const
{
	switch (m_op)
	{
	case UnionOp:
	{
		std::string glsl = "";
		for (size_t i = 0; i < childCount(); ++i)
		{
			std::string childGlsl = child(i)->compileToGlsl();
			glsl = i == 0 ? childGlsl : ("opU(" + glsl + ", " + childGlsl + ")");
		}
		return target.empty() ? glsl : ("vec2 " + target + " = " + glsl + ";");
	}
	case DifferenceOp:
	{
		std::string glsl = "";
		for (size_t i = 0; i < childCount(); ++i)
		{
			std::string childGlsl = child(i)->compileToGlsl();
			glsl = i == 0 ? childGlsl : ("opD(" + glsl + ", " + childGlsl + ")");
		}
		return target.empty() ? glsl : ("vec2 " + target + " = " + glsl + ";");
	}
	case IntersectionOp:
		ERR_LOG << "Intersection Operator Node not implemented";
		return "";
	default:
		ERR_LOG << "Unknown Scene Operator Node";
		return "";
	}
}

int SceneOperationNode::maxChildren() const
{
	switch (m_op)
	{
	case UnionOp:
		return -1;
	case DifferenceOp:
		return 2;
	case IntersectionOp:
		return -1;
	default:
		return -1;
	}
}

std::string ScenePrimitiveNode::compileToGlsl(std::string target) const
{
	return target.empty() ? source() : ("vec2 " + target + " = " + source() + ";");
}
