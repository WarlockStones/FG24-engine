#include "EntityModel.hpp"
#include "EntityManager.hpp"

EntityModel::EntityModel(QObject* parent, EntityManager* entityManager)
: QAbstractListModel(parent), entityManager(entityManager) {
	defaultFont.setPointSize(14);
}

int EntityModel::rowCount(const QModelIndex& parent) const {
	if (entityManager) {
		return entityManager->GetEntityCount();
	} 

	return 0;
}

QVariant EntityModel::data(const QModelIndex& index, int role) const {
	auto row = index.row();
	switch (role) {
	case Qt::DisplayRole:
		if (entityManager) {
		    // Entities should always be listed in alphabetical order
			return entityManager->GetEntityNameAtIndex(index.row());
		}
	case Qt::FontRole:
		return defaultFont;
	case Qt::BackgroundRole:
		if (row % 2) {
			// TODO: This should take into account darkmode
			// return QBrush(Qt::lightGray);
		}
	default:
		break;
	}

	return QVariant();
}

bool EntityModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	// TODO: Implement this. Maybe it should only change name?
	return false;
}

Qt::ItemFlags EntityModel::flags(const QModelIndex& index) const {
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}
