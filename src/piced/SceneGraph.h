#pragma once

#include <string>
#include <vector>

class SceneTree
{
public:
	enum SceneNodeType
	{
		OperationNode,
		PrimitiveNode
	};

public:
	explicit SceneTree(SceneTree *parent = nullptr) : m_parent(parent)
	{}

	inline SceneTree *parent() const { return m_parent; }
	inline SceneNodeType type() const { return m_type; }
	inline size_t childCount() const { return m_children.size(); }
	inline SceneTree *child(size_t index) { return &m_children.at(index); }

	inline std::string name() const { return m_name; }
	inline void setName(const std::string & name) { m_name = name; }

	inline bool isRoot() const { return m_parent == nullptr; }

	void addChild() { m_children.push_back(SceneTree(this)); }

private:
	SceneTree *m_parent;
	SceneNodeType m_type;
	std::vector<SceneTree> m_children;
	std::string m_name;
};

