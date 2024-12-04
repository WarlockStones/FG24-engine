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

	// Test entity properties
	for (const auto& entity : em.entities) {
		auto name = entity.GetValue("classname");
		if (name) {
			std::cout<<"name: "<< name.value() << '\n';
		}
		auto origin = entity.GetValue("origin");
		if (origin) {
			std::cout << "origin: " << origin.value() << '\n';
		}
		auto bad = entity.GetValue("bad");
		if (bad) {
			std::cout << "bad\n";
		}
	}

	// TODO: add proper selection
	em.selectedEntity = em.entities[0]; // Testing selection


	// The UI 
	QApplication app(argc, argv);

	// Main window
	MainWindow window(nullptr, &em);
	window.show();

	return app.exec(); // Pass control and start Qt event loop
} 
