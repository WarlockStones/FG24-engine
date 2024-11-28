#include <QtWidgets>
#include <cstdio>

int main(int argc, char* argv[]) {
	int i = 0;
	QApplication app(argc, argv);
	QWidget window;
	window.resize(320, 240);
	window.show();
	window.setWindowTitle(
		QApplication::translate("toplevel", "Top-level widget"));
	std::printf("end of InitQt\n");
	app.exec();
} 
