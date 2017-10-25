#include "SceneGraph.h"

#include "Logger.h"

SceneTree::SceneTree(SceneTree *parent)
	: m_parent(parent)
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
