#include <qplexymime.h>

#include <QCoreApplication>
#include <QtDebug>

int main (int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	QPlexyMime mime;
	qDebug() << "From file name:" << mime.fromFileName("test.rtf");
	qDebug() << "Gneric icon name:" << mime.genericIconName();
	qDebug() << "Expanded acronym:" << mime.expandedAcronym();
	qDebug() << "Description:" << mime.description();
	qDebug() << "Description localized (uk):" << mime.description("uk");
	qDebug() << "Subclass Of:" << mime.subClassOf();
	qDebug() << "Mime type:" << mime.mimeType();
	qDebug() << "Acronym:" << mime.acronym();
	qDebug() << "Alias:" << mime.alias();
}
