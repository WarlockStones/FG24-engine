#include "EntityDelegate.hpp"
#include <QModelIndex>
#include <QPainter>
#include "Entity.hpp"

void EntityDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  /*
	if (index.data().canConvert<Entity>()) {
		Entity e = qvariant_cast<Entity>(index.data());

		if (option.state & QStyle::State_Selected) {
			painter->fillRect(option.rect, option.palette.highlight());
		}
			
		// Paint the contents of e
		painter->drawText(option.rect, QString::fromStdString(e.name));
		

	} else {
		QStyledItemDelegate::paint(painter, option, index);
	}
*/
}

QSize EntityDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return QSize();
}
