#pragma once
#include <QAbstractTableModel>

class EntityManager;
class EntityPropertyModel : public QAbstractTableModel {
	Q_OBJECT

public:
	explicit EntityPropertyModel(QObject* parent = nullptr, EntityManager* entityManager = nullptr);
	~EntityPropertyModel() = default;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
	EntityManager* entityManager;
};
