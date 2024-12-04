#include "MainWindow.hpp"
#include "EntityModel.hpp"
#include "EntityPropertyModel.hpp"
#include "EntityManager.hpp"

#include <cstdio>
#include <cassert>

#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QListView>
#include <QTableView>

MainWindow::MainWindow(QWidget* parent, EntityManager* entityManager) 
: entityManager(entityManager)
, entityModel(new EntityModel(this, entityManager))
, entityPropertyModel(new EntityPropertyModel(this, entityManager))
, listView(new QListView(this))
, propertiesView(new QTableView(this)) {
	assert(entityManager);

	this->resize(800, 600);
	this->setWindowTitle("World Editor");

	listView->setModel(entityModel);
	listView->setSelectionRectVisible(true);
	listView->setUniformItemSizes(true); // Improves performance if true
	listView->show();

	propertiesView->setModel(entityPropertyModel);
	propertiesView->setSelectionBehavior(QAbstractItemView::SelectItems);
	propertiesView->setSelectionMode(QAbstractItemView::SingleSelection);

	QSplitter* splitter = new QSplitter(this);
	setCentralWidget(splitter); // Central widget changes layout
	splitter->setOrientation(Qt::Vertical);
	splitter->setStretchFactor(1, 1);
	splitter->addWidget(listView);
	splitter->addWidget(propertiesView);
	
	
	// Delegates
	QItemSelectionModel* selectionModel = listView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged,
			this, &MainWindow::selectionChangedSlot);
}

MainWindow::~MainWindow() {
	delete entityModel;
	delete entityPropertyModel;
	delete listView;
	delete propertiesView;
}

void MainWindow::selectionChangedSlot(const QItemSelection& newSelection,
									  const QItemSelection& oldSelection) {

	const QModelIndex& index = listView->selectionModel()->currentIndex();
	entityManager->selectedEntity = entityManager->entities[index.row()];

	propertiesView->viewport()->update();
}
