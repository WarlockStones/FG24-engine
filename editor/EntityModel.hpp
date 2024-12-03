#pragma once
#include <QListView>
#include <QAbstractListModel>
#include <vector>

class EntityManager;

// Model is the wrapper around the data. It must implement some functions
class EntityModel : public QAbstractListModel {
	Q_OBJECT

public:
    // ListModel must implement: Constructor, rowCount(), data()
	explicit EntityModel (QObject* parent = nullptr, EntityManager* entityManager = nullptr);
    ~EntityModel() = default;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable list models must implement setData(), flags() to return a
	// value containing Qt::ItemIsEditable
	bool setData(const QModelIndex& index, const QVariant& value,
				 int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
	EntityManager* entityManager = nullptr;
	QFont defaultFont;
};
