#include <qplexymime.h>

#include <QCoreApplication>
#include <QtDebug>

int main (int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QPlexyMime mime;
	qDebug() << mime.fromFileName("test.pdf");
	qDebug() << mime.genericIconName();
	qDebug() << mime.expandedAcronym();
}
