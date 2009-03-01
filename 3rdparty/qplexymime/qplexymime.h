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

#include <QObject>

class QString;
class QFile;

class QPlexyMime : public QObject
{
	Q_OBJECT

public:
	QPlexyMime (QObject *parent = 0);
	~QPlexyMime();

	QString fromFileName (const QString& fileName);
/*
	QString fromFile (const QString& fileName);
	QString fromFile (QFile *file);
*/
	QString genericIconNameMime (const QString& mimeType);
	QString expandedAcronymMime (const QString& mimeType);
	QString descriptionMime (const QString& mimeType, const QString &lang = QString());
	QString subClassOfMime (const QString& mimeType);
	QString acronymMime (const QString& mimeType);
	QString aliasMime (const QString& mimeType);

	QString genericIconName (void) const;
	QString expandedAcronym (void) const;
	QString description (const QString &lang = QString()) const;
	QString subClassOf (void) const;
	QString mimeType (void) const;
	QString acronym (void) const;
	QString alias (void) const;

private:
	class QPlexyMimePrivate;
	QPlexyMimePrivate *d;
};


