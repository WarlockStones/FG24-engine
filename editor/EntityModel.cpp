#include "EntityModel.hpp"
#include "EntityManager.hpp"

EntityModel::EntityModel(QObject* parent)
	: QAbstractListModel(parent){
}

int EntityModel::rowCount(const QModelIndex& parent) const {
	if(entityManager) {
		return entityManager->GetEntityCount();
	} 

	return 0;
}

QVariant EntityModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::DisplayRole && entityManager) {
	    return entityManager->GetEntityNameAtIndex(index.row());
	}

	return QVariant();
}

QVariant EntityModel::headerData(int section, Qt::Orientation, int role) const {
	// TODO: Implement this
	return QVariant();
}

bool EntityModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	// TODO: Implement this
	return false;
}

Qt::ItemFlags EntityModel::flags(const QModelIndex& index) const {
  // TODO: Implement this
  return static_cast<Qt::ItemFlags>(0);
}
