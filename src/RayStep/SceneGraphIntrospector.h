#ifndef H_SCENEGRAPHINTROSPECTOR
#define H_SCENEGRAPHINTROSPECTOR

#include "SceneGraph.h"

#include <QString>
#include <QVariant>

#include <memory>
#include <vector>

/**
 * Introspectors are intermediate classes that proposes a unified interfaces to Scene node classes.
 * It is used by the SceneTreeModel to provide node properties through the model's column, whichever the actual node type is.
 * It connects Qt-based interface to the framework-agnostic SceneGraph
 *
 * TODO: There is a lot of factorization work to be done here, because this set of classes is inheritently verbose
 */

class SceneTreeIntrospector
{
public:
	SceneTreeIntrospector(SceneTree *node) : m_node(node) {}

	virtual int nbProperties() const;
	virtual QString property(int index) const;
	virtual QVariant value(int index) const;
	virtual bool setValue(int index, QVariant value);
	// TODO: add Type

protected:
	inline SceneTree *node() const { return m_node; }

private:
	SceneTree *m_node;
};

class SceneOperationNodeIntrospector : public SceneTreeIntrospector
{
public:
	SceneOperationNodeIntrospector(SceneOperationNode *node) : SceneTreeIntrospector(node) {}

	virtual int nbProperties() const;
	virtual QString property(int index) const;
	virtual QVariant value(int index) const;
	virtual bool setValue(int index, QVariant value);

protected:
	inline SceneOperationNode* operationNode() const { return static_cast<SceneOperationNode*>(node()); }
};

class ScenePrimitiveNodeIntrospector : public SceneTreeIntrospector
{
public:
	ScenePrimitiveNodeIntrospector(ScenePrimitiveNode *node) : SceneTreeIntrospector(node) {}

	virtual int nbProperties() const;
	virtual QString property(int index) const;
	virtual QVariant value(int index) const;
	virtual bool setValue(int index, QVariant value);

protected:
	inline ScenePrimitiveNode* primitiveNode() const { return static_cast<ScenePrimitiveNode*>(node()); }
};

class SceneTreeIntrospectorFactory
{
public:
	static std::unique_ptr<SceneTreeIntrospector> makeIntrospector(SceneTree *node);
};

#endif // H_SCENEGRAPHINTROSPECTOR
