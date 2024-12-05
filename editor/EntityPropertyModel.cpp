#include "EntityPropertyModel.hpp"
#include "EntityManager.hpp"
#include <cassert>
#include <vector>
#include <string_view>
#include <iostream> // For debug logging

EntityPropertyModel::EntityPropertyModel(QObject* parent, EntityManager* entityManager)
: QAbstractTableModel(parent), entityManager(entityManager) {
	assert(entityManager);
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

QVariant EntityPropertyModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString("Property");
		case 1:
			return QString("Value");
		}
	}

	return QVariant();
}

bool EntityPropertyModel::setData(const QModelIndex& index, const QVariant &value,
								  int role) {
	if (role == Qt::EditRole) {
		if (!checkIndex(index)) {
			return false;
		}
		std::string input = value.toString().toStdString();
		if (input == "") {
			return false;
		}

		// TODO: Update method of getting and setting selected properties.
		//       Needs checks if overriding, or adding to empty, etc.
		EntityProperty& selectedProp
		  = entityManager->selectedEntity.value().get().properties[index.row()];

		if (index.column() == 0) {
			// Change property name
			selectedProp.key = input;
			return true;

		} else if (index.column() == 1) {
			// Change property value
		  selectedProp.value = input;
			// entityManager->selectedEntity.AddProperty()
			return true;
		} 
	}

	std::cout<<"Something probably went wrong in EntityPropertyModel::setData\n";
	std::cout<<"Could not setData\n";
	return false;
}

Qt::ItemFlags EntityPropertyModel::flags(const QModelIndex& index) const {
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
