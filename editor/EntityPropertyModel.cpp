#include "EntityPropertyModel.hpp"
#include "EntityManager.hpp"

EntityPropertyModel::EntityPropertyModel(QObject* parent, EntityManager* entityManager) {

}

QVariant EntityPropertyModel::data(const QModelIndex& index, int role) const {
	return QVariant();
}

int EntityPropertyModel::rowCount(const QModelIndex& parent) const {
	// Get the length of properties in the selected entity

	return 4;
}
int EntityPropertyModel::columnCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return 2;
}
