/* 
 *  This file is part of Quartica.
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Quartica is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Quartica.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

#include "freedesktopmime.h"

int main (int argc, char **argv) {
	QCoreApplication app(argc, argv);

	QFreeDesktopMime mime;
	qDebug() << mime.genericIconName("application/pkcs7-mime");
	qDebug() << mime.description("application/epub+zip");
	qDebug() << mime.expandedAcronym("text/x-xslfo");
	qDebug() << mime.description("text/x-xslfo");
	qDebug() << mime.acronym("text/x-xslfo");
	qDebug() << mime.alias("text/x-xslfo");

	qDebug() << mime.fromFileName("test.jpg");
	qDebug() << mime.description();
	qDebug() << mime.fromFileName("test.png");
	qDebug() << mime.description();
	qDebug() << mime.fromFileName("test.txt");
	qDebug() << mime.description();
	qDebug() << mime.fromFileName("test.cpp");
	qDebug() << mime.description();
	qDebug() << mime.fromFileName("test.h");
	qDebug() << mime.description();

	if (argc > 1) {
		if (QFileInfo(argv[1]).isDir()) {
			QDir dir(argv[1]);
			//dir.setFilter(QDir::Files);
			foreach (QFileInfo fileInfo, dir.entryInfoList())
				qDebug() << "-" << mime.fromFile(fileInfo.absoluteFilePath()) << fileInfo.fileName();
		} else {
			qDebug() << "-" << mime.fromFile(argv[1]) << argv[1];
		}
	}

	return(0);
}

