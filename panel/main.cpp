#include <QApplication>
#include "plexypanel.h"
int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	PlexyPanel *panel = new PlexyPanel();
	return app.exec();
}
