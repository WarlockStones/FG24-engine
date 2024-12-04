#include "EntityPropertyModel.hpp"
#include "EntityManager.hpp"
#include <cassert>
#include <vector>

EntityPropertyModel::EntityPropertyModel(QObject* parent, EntityManager* entityManager)
: QAbstractTableModel(parent), entityManager(entityManager) {
	assert(entityManager);
}

QVariant EntityPropertyModel::data(const QModelIndex& index, int role) const {
	int row = index.row();
	int col = index.column();

	if (role == Qt::DisplayRole) {
		if (!entityManager->selectedEntity.has_value()) {
			return QVariant(); // No properties to display
		}
		const auto& propertyAtIndex = entityManager->selectedEntity.value().get().properties[index.row()];

		if (col == 0) { // Property key
			return QString::fromStdString(propertyAtIndex.key);
		} else if (col == 1) { // Property value
			return QString::fromStdString(propertyAtIndex.value);
		}
	}

	return QVariant();
}

int EntityPropertyModel::rowCount(const QModelIndex& parent) const {
	if (entityManager->selectedEntity.has_value()) {
		return entityManager->selectedEntity.value().get().properties.size();
	} else {
		return 0;
	}
}
int EntityPropertyModel::columnCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return 2; // key : value
}
