#include "Editor.hpp"
#include <QtWidgets>
#include <cstdio>


namespace FG24 {
void Editor::InitQt() {
	int i = 0;
	QApplication app(i, nullptr);
	QWidget window;
	window.resize(320, 240);
	window.show();
	window.setWindowTitle(
		QApplication::translate("toplevel", "Top-level widget"));
	std::printf("end of InitQt\n");
	app.exec();
} 
} // namespace FG24
