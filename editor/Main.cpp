#include <QtWidgets>
#include <QApplication>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "EntityModel.hpp"
#include "EntityManager.hpp"

int main(int argc, char* argv[]) {
	// The data
	EntityManager em;
	em.Init();

	// The UI
	QApplication app(argc, argv);
	EntityModel entityModel;
	entityModel.entityManager = &em;
	QListView listView;
	listView.setModel(&entityModel);
	listView.setSelectionRectVisible(true);
	listView.setUniformItemSizes(true); // Improves performance
	listView.show();

	/*
	QWidget window;
	window.resize(800, 600);
	window.show();
	window.setWindowTitle(
		QApplication::translate("toplevel", "Top-level widget"));
	*/

	return app.exec(); // Pass control and start Qt event loop
} 
