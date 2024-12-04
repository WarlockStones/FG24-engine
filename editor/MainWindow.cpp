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
, listView(new QListView(this)) {
	assert(entityManager);

	this->resize(800, 600);
	this->setWindowTitle("World Editor");

	listView->setModel(entityModel);
	listView->setSelectionRectVisible(true);
	listView->setUniformItemSizes(true); // Improves performance if true
	listView->show();

	// TODO: add splitter for details view
	// TODO: add these as member variables
	// It should be a listView with a custom itemDelegate to handle component data
	auto table = new QTableView(this);
	table->setModel(entityPropertyModel);
	table->setSelectionBehavior(QAbstractItemView::SelectItems);
	table->setSelectionMode(QAbstractItemView::SingleSelection);

	QSplitter* splitter = new QSplitter(this);
	setCentralWidget(splitter); // Central widget changes layout
	splitter->setOrientation(Qt::Vertical);
	splitter->setStretchFactor(1, 1);
	splitter->addWidget(listView);
	splitter->addWidget(table);
	
	
	// delegates
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
