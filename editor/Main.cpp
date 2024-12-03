#include <QtWidgets>
#include <QApplication>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "EntityManager.hpp"
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
	// The data
	EntityManager em;
	em.Init();

	for (const auto& entity : em.entities) {
		auto name = entity.GetValue("classname");
		if (name) {
			std::cout<<name.value()<<'\n';
		}
	}

	// The UI 
	QApplication app(argc, argv);

	// Main window
	MainWindow window(nullptr, &em);
	window.show();

	// Selection



	/*
	QWidget window;
	window.resize(800, 600);
	window.show();
	window.setWindowTitle(
		QApplication::translate("toplevel", "Top-level widget"));
	*/

	return app.exec(); // Pass control and start Qt event loop
} 
