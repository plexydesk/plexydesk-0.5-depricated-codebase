/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Dariusz Mikulski <dariusz.mikulski@gmail.com>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include "testmime.h"

#include <QCoreApplication>
#include <QtDebug>

int main (int argc, char **argv)
{
	QCoreApplication app(argc, argv);
        using namespace PlexyDesk;

        TestMime mime(&app);

        return app.exec();
/*
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
*/
}
