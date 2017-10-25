#include "SceneGraph.h"

#include "Logger.h"

std::string SceneTree::compileToGlsl(std::string target) const
{
	DEBUG_LOG << "toGLSL: " << target << ", " << name();
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
			return m_children[0].compileToGlsl();
		}
		else
		{
			return m_children[0].compileToGlsl("content") + "\n" + target + " = opU(" + target + ", content);";
		}
	}

	switch (type())
	{
	case SceneTree::PrimitiveNode:
		return target.empty() ? name() : ("vec2 " + target + " = " + name() + ";");
		break;

	case SceneTree::OperationNode:
	{
		if (name() == "Union")
		{
			std::string glsl = "";
			for (size_t i = 0 ; i < m_children.size() ; ++i)
			{
				const SceneTree & child = m_children.at(i);
				std::string childGlsl = child.compileToGlsl();
				glsl = i == 0 ? childGlsl : ("opU(" + glsl + ", " + childGlsl + ")");
			}
			return target.empty() ? glsl : ("vec2 " + target + " = " + glsl + ";");
		}
		else
		{
			ERR_LOG << "Unknown Scene Operator Node: " << name();
		}
		break;
	}

	default:
		ERR_LOG << "Unknown Scene Operator Node Type: " << type();
		break;
	}
}

/*
scene = opU( scene, vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 ) );
scene = opU( scene, vec2( sdBox(       pos-vec3( 1.0,0.25, 0.0), vec3(0.25) ), 3.0 ) );
scene = opU( scene, vec2( udRoundBox(  pos-vec3( 1.0,0.25, 1.0), vec3(0.15), 0.1 ), 41.0 ) );
*/