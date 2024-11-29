#include "MainWindow.hpp"
#include "EntityModel.hpp"
#include "EntityManager.hpp"

#include <cstdio>
#include <cassert>

MainWindow::MainWindow(QWidget* parent, EntityManager* entityManager) 
: entityManager(entityManager)
, entityModel(new EntityModel(this, entityManager))
, listView(new QListView(this)) {
	assert(entityManager);

	setCentralWidget(listView);

	listView->setModel(entityModel);
	listView->setSelectionRectVisible(true);
	listView->setUniformItemSizes(true); // Improves performance
	listView->show();
	
	QItemSelectionModel* selectionModel = listView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged,
			this, &MainWindow::selectionChangedSlot);
}

MainWindow::~MainWindow() {
	delete entityModel;
	delete listView;
}

void MainWindow::selectionChangedSlot(const QItemSelection& newSelection,
									  const QItemSelection& oldSelection) {

	const QModelIndex index = listView->selectionModel()->currentIndex();
	QString selectedText = index.data(Qt::DisplayRole).toString();

	setWindowTitle(
	  index.data(Qt::DisplayRole).toString()
	);
}
