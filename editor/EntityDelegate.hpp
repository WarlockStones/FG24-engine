#pragma once
#include <QStyledItemDelegate>

class EntityDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	// We must tell Qt how to paint all contents of an Entity
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
				const QModelIndex& index) const override;

	QSize sizeHint(const QStyleOptionViewItem& option,
				const QModelIndex& index) const override;

	// Editor ?


};
