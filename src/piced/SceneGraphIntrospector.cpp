#include "SceneGraphIntrospector.h"

int SceneTreeIntrospector::nbProperties() const
{
	return 1;
}

QString SceneTreeIntrospector::property(int index) const
{
	switch (index)
	{
	case 0:
		return "Name";
	default:
		return QString();
	}
}

QVariant SceneTreeIntrospector::value(int index) const
{
	switch (index)
	{
	case 0:
		return QString::fromStdString(node()->name());
	default:
		return QVariant();
	}
}

bool SceneTreeIntrospector::setValue(int index, QVariant value)
{
	switch (index)
	{
	case 0:
		if (!value.canConvert<QString>())
		{
			return false;
		}
		node()->setName(value.toString().toStdString());
		break;
	default:
		return false;
	}
	return true;
}


int SceneOperationNodeIntrospector::nbProperties() const
{
	return SceneTreeIntrospector::nbProperties() + 1;
}

QString SceneOperationNodeIntrospector::property(int index) const
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::property(index);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		return "Operation";
	default:
		return QString();
	}
}

QVariant SceneOperationNodeIntrospector::value(int index) const
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::value(index);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		return operationNode()->operation();
	default:
		return QVariant();
	}
}

bool SceneOperationNodeIntrospector::setValue(int index, QVariant value)
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::setValue(index, value);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		if (!value.canConvert<int>())
		{
			return false;
		}
		operationNode()->setOperation(static_cast<SceneOperationNode::OperationType>(value.toInt()));
		break;
	default:
		return false;
	}
	return true;
}


int ScenePrimitiveNodeIntrospector::nbProperties() const
{
	return SceneTreeIntrospector::nbProperties() + 1;
}

QString ScenePrimitiveNodeIntrospector::property(int index) const
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::property(index);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		return "Source";
	default:
		return QString();
	}
}

QVariant ScenePrimitiveNodeIntrospector::value(int index) const
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::value(index);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		return QString::fromStdString(primitiveNode()->source());
	default:
		return QVariant();
	}
}

bool ScenePrimitiveNodeIntrospector::setValue(int index, QVariant value)
{
	int baseNbProperties = SceneTreeIntrospector::nbProperties();
	if (index < baseNbProperties)
	{
		return SceneTreeIntrospector::setValue(index, value);
	}

	switch (index - baseNbProperties)
	{
	case 0:
		if (!value.canConvert<QString>())
		{
			return false;
		}
		primitiveNode()->setSource(value.toString().toStdString());
		break;
	default:
		return false;
	}
	return true;
}

std::unique_ptr<SceneTreeIntrospector> SceneTreeIntrospectorFactory::makeIntrospector(SceneTree *node)
{
	switch (node->type())
	{
	case SceneTree::OperationNode:
		return std::unique_ptr<SceneTreeIntrospector>(new SceneOperationNodeIntrospector(static_cast<SceneOperationNode*>(node)));
	case SceneTree::PrimitiveNode:
		return std::unique_ptr<SceneTreeIntrospector>(new ScenePrimitiveNodeIntrospector(static_cast<ScenePrimitiveNode*>(node)));
	default:
		return std::unique_ptr<SceneTreeIntrospector>(new SceneTreeIntrospector(node));
	}
}


