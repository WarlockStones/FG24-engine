#include "MainWindow.hpp"
#include "EntityModel.hpp"
#include "EntityManager.hpp"

#include <cstdio>
#include <cassert>

#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent, EntityManager* entityManager) 
: entityManager(entityManager)
, entityModel(new EntityModel(this, entityManager))
, listView(new QListView(this)) {
	assert(entityManager);

	listView->setModel(entityModel);
	listView->setSelectionRectVisible(true);
	listView->setUniformItemSizes(true); // Improves performance if true
	listView->show();

	// TODO: add splitter for details view
	// TODO: add these as member variables

	QLabel* label = new QLabel(this);
	label->setText("This is a test");

	QSplitter* splitter = new QSplitter(this);
	setCentralWidget(splitter); // Central widget changes layout
	splitter->setOrientation(Qt::Vertical);
	splitter->setStretchFactor(1, 1);
    splitter->addWidget(listView);
    splitter->addWidget(label);

	
	
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
