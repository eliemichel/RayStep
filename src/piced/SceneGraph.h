#ifndef H_SCENEGRAPH
#define H_SCENEGRAPH

#include <string>
#include <vector>

/**
 * SceneTree is a structure holding a subtree, namely its node and a pointer to each of its subtrees.
 * A SceneTree object is responsible for freeing its children.
 * I did not create an extra indirection for the node data, because SceneTree objects are already only refered by pointers.
 * To create a special type of node, just inherit from SceneTree.
 *
 * Invariants:
 *  - There is always one and only one SceneTree* pointing to a given SceneTree object among the m_children of all SceneTrees, except for the root one
 *  - tree->m_parent->m_children contains tree unless tree is root
 *  - If tree->m_type if FooNode, then tree can be safely static_casted into a SceneFooNode
 */
class SceneTree
{
public:
	enum SceneNodeType
	{
		OperationNode,
		PrimitiveNode
	};

public:
	explicit SceneTree(SceneTree *parent = nullptr);
	~SceneTree();

	inline SceneTree *parent() const { return m_parent; }

	inline SceneNodeType type() const { return m_type; }
	inline void setType(SceneNodeType type) { m_type = type; }

	inline size_t childCount() const { return m_children.size(); }
	inline SceneTree *child(size_t index) { return m_children.at(index); }
	inline const SceneTree *child(size_t index) const { return m_children.at(index); }

	inline std::string name() const { return m_name; }
	inline void setName(const std::string & name) { m_name = name; }

	inline bool isRoot() const { return m_parent == nullptr; }
	inline std::string internalPath() const { return isRoot() ? "/" : parent()->internalPath() + "/" + name(); }

	/// Add an existing node
	/// The node is then managed and will be freed by the new parent SceneTree
	/// Insert child before index beforeIndex, except if the latter is negative
	void addChild(SceneTree *node, int beforeIndex = -1);
	/// Add a new node and return a pointer to it
	/// NodeType must be a subclass of SceneTree
	template <typename NodeType>
	inline NodeType * addChild(int beforeIndex = -1) { NodeType *node = new NodeType(this); addChild(node, beforeIndex); return node; }
	inline SceneTree * addChild(int beforeIndex = -1) { return addChild<SceneTree>(beforeIndex); }

	// Free and remove child
	void removeChild(int childIndex);
	// Remove child from children list without deleting it, returns a pointer to it
	SceneTree *takeChild(int childIndex);

	/// @param target: Name of the glsl variable in which storing the value
	/// leave empty to inline the expression
	/// (not injection safe, do not expose to user raw input)
	virtual std::string compileToGlsl(std::string target = "") const;

	/// Used by subclasses to bind the number of children.
	/// Typically, this is 0 for primitives and 1 for unary operators
	/// Leave it to -1 to mean unlimited
	virtual int maxChildren() const { return -1; }

private:
	inline void setParent(SceneTree *parent){ m_parent = parent; }

private:
	SceneTree *m_parent;
	std::vector<SceneTree*> m_children;

	SceneNodeType m_type;
	std::string m_name;
};

class SceneOperationNode : public SceneTree
{
public:
	enum OperationType
	{
		UnionOp,
		DifferenceOp,
		IntersectionOp
	};
public:
	SceneOperationNode(SceneTree *parent = nullptr) : SceneTree(parent) { setType(OperationNode); setName("<operation>"); }

	inline OperationType operation() const { return m_op; }
	inline void setOperation(OperationType op) { m_op = op; }

	virtual std::string compileToGlsl(std::string target = "") const override;

	virtual int maxChildren() const override;

private:
	OperationType m_op;
};


class ScenePrimitiveNode : public SceneTree
{
public:
	ScenePrimitiveNode(SceneTree *parent = nullptr) : SceneTree(parent) { setType(PrimitiveNode); setName("<primitive>"); }

	inline std::string source() const { return m_source; }
	inline void setSource(const std::string & source) { m_source = source; }

	virtual std::string compileToGlsl(std::string target = "") const override;

	virtual int maxChildren() const override { return 0; }

private:
	std::string m_source;
};

#endif // H_SCENEGRAPH
