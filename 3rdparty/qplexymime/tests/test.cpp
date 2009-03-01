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

	qDebug() << "***********************************************";
        qDebug() << "Gneric icon name:" << mime.genericIconNameMime("application/rtf");
        qDebug() << "Expanded acronym:" << mime.expandedAcronymMime("application/rtf");
        qDebug() << "Description:" << mime.descriptionMime("application/rtf");
        qDebug() << "Description localized (uk):" << mime.descriptionMime("application/rtf", "uk");
        qDebug() << "Subclass Of:" << mime.subClassOfMime("application/rtf");
        qDebug() << "Acronym:" << mime.acronymMime("application/rtf");
        qDebug() << "Alias:" << mime.aliasMime("application/rtf");
}
