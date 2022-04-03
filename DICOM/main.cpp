#include <QGuiApplication>
#include <QtWidgets/QApplication>
#include "MainWidget.h"
int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QApplication a(argc, argv);

	MainWidget mWidget;
	mWidget.show();

	a.exec();
	return 0;
}
