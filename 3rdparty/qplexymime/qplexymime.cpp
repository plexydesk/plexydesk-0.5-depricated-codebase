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

/*
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>
#include <QtXmlPatterns/QSimpleXmlNodeModel>
#include <QtXmlPatterns/QXmlFormatter>

int main (int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	
	QFile sourceDocument("freedesktop.org.xml");
	if(!sourceDocument.open(QIODevice::ReadOnly))
		return 0;
	
	QString newQuery("declare namespace ns = 'http://www.freedesktop.org/standards/shared-mime-info';\ndeclare variable $internalFile external;\n");
	newQuery += QString("doc($internalFile)/ns:mime-info/ns:mime-type[@type='application/andrew-inset']/ns:comment/string()");
	
	QByteArray output = sourceDocument.readAll();

    QBuffer outputBuffer(&output);
    outputBuffer.open(QIODevice::ReadOnly);	
	
	QXmlQuery query;
	query.bindVariable("internalFile", &outputBuffer);
	query.setQuery(newQuery);
	
	if(!query.isValid())
	{
		qDebug() << "Query is not valid";
		return 0;
	}

	QStringList result;
	query.evaluateTo(&result);
	
	qDebug() << result;
					 
*/

#include "qplexymime.h"

#include <QString>
#include <QFile>

QPlexyMime::QPlexyMime (QObject *parent)
{
}

QPlexyMime::~QPlexyMime()
{
}

QString QPlexyMime::fromFileName (const QString& fileName)
{
	return QString();
}

QString QPlexyMime::fromFile (const QString& fileName)
{
	return QString();
}

QString QPlexyMime::fromFile (QFile *file)
{
	return QString();
}

QString QPlexyMime::genericIconName (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::expandedAcronym (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::description (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::subClassOf (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::acronym (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::alias (const QString& mimeType)
{
	return QString();
}

QString QPlexyMime::genericIconName (void) const
{
	return QString();
}

QString QPlexyMime::expandedAcronym (void) const
{
	return QString();
}

QString QPlexyMime::description (void) const
{
	return QString();
}

QString QPlexyMime::subClassOf (void) const
{
	return QString();
}

QString QPlexyMime::mimeType (void) const
{
	return QString();
}

QString QPlexyMime::acronym (void) const
{
	return QString();
}

QString QPlexyMime::alias (void) const
{
	return QString();
}

