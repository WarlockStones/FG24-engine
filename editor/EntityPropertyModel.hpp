#pragma once
#include <QAbstractTableModel>

class EntityManager;
class EntityPropertyModel : public QAbstractTableModel {
	Q_OBJECT

public:
	explicit EntityPropertyModel(QObject* parent = nullptr, EntityManager* entityManager = nullptr);
	~EntityPropertyModel() = default;

	QVariant data(const QModelIndex& index, int role) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};
