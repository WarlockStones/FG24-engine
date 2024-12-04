#include "EntityDelegate.hpp"
#include <QModelIndex>
#include <QPainter>
#include "Entity.hpp"

void EntityDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	if (index.data().canConvert<const Entity*>()) {
		const auto* e = qvariant_cast<const Entity*>(index.data());

		if (option.state & QStyle::State_Selected) {
			painter->fillRect(option.rect, option.palette.highlight());
		}
			
		// Paint the contents of entity at index
		// painter->drawText(option.rect, QString::fromStdString(e->GetName()));
		painter->drawText(option.rect, QString("Hello"));

		

	} else {
		QStyledItemDelegate::paint(painter, option, index);
	}
}

QSize EntityDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return QSize();
}
