#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>

#include "TestWidgets.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtWidgetsApplication1 w;
	w.show();
	auto hwnd = w.winId();
	return a.exec();
}
