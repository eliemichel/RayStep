#ifndef H_UNIFORMSMODEL
#define H_UNIFORMSMODEL

#include <QAbstractItemModel>
#include <QList>

#include <string>
#include <vector>

class UniformsModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	struct Uniform
	{
		std::string name;
		float value;

		Uniform(const std::string & _name, float _value) { name = _name; value = _value; }
	};

public:
	explicit UniformsModel(QObject *parent = Q_NULLPTR);
	~UniformsModel();

	// Read access
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Editable items
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
	QList<Uniform> m_uniforms;
};


#endif // H_UNIFORMSMODEL
