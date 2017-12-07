#include "UniformsModel.h"
#include "Logger.h"

UniformsModel::UniformsModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	m_uniforms << Uniform("uFloat0", 0.0f);
	m_uniforms << Uniform("uFloat1", 0.0f);
	m_uniforms << Uniform("uFloat2", 0.0f);
}

UniformsModel::~UniformsModel()
{}

QModelIndex UniformsModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!parent.isValid())
	{
		return createIndex(row, column, nullptr);
	}
	return QModelIndex();
}

QModelIndex UniformsModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

int UniformsModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
	{
		return m_uniforms.size();
	}
	return 0;
}

int UniformsModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant UniformsModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		if (index.row() < 0 || index.row() >= m_uniforms.size())
		{
			ERR_LOG << "Access to invalid row in uniforms model";
			return QVariant();
		}

		switch (index.column())
		{
		case 0:
			return QString::fromStdString(m_uniforms[index.row()].name);
		case 1:
			return m_uniforms[index.row()].value;
		default:
			ERR_LOG << "Access to invalid column in uniforms model";
			return QVariant();
		}
	}
	return QVariant();
}


QVariant UniformsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return "Uniform";
		case 1:
			return "Value";
		default:
			return QVariant();
		}
	}
	return QVariant();
}


// Editable items
Qt::ItemFlags UniformsModel::flags(const QModelIndex &index) const
{
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool UniformsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		if (index.row() < 0 || index.row() >= m_uniforms.size())
		{
			ERR_LOG << "Write to invalid row in uniforms model";
			return false;
		}

		switch (index.column())
		{
		case 0:
			if (!value.canConvert<QString>())
			{
				ERR_LOG << "Uniform name (column 0) must be a string";
				return false;
			}
			m_uniforms[index.row()].name = value.toString().toStdString();
			return true;
		case 1:
			if (!value.canConvert<float>())
			{
				ERR_LOG << "Uniform value (column 1) must be a float (for now)";
				return false;
			}
			m_uniforms[index.row()].value = value.toFloat();
			valueChanged(m_uniforms[index.row()].name, value.toFloat());
			return true;
		default:
			ERR_LOG << "Write to invalid column in uniforms model";
			return false;
		}
	}
	return false;
}

std::string UniformsModel::compileToGlsl() const
{
	std::ostringstream ss;
	for (int i = 0 ; i < rowCount() ; ++i)
	{
		ss << "uniform float " << data(index(i, 0)).toString().toStdString() << " = " << data(index(i, 1)).toFloat() << ";";
	}
	return ss.str();
}
