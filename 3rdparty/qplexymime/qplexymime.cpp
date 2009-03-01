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
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QtXmlPatterns/QXmlQuery>
#include <QtDebug>

class QPlexyMime::QPlexyMimePrivate
{
private:
	QString newQuery;

public:
	QByteArray output;
	QBuffer outputBuffer;
	QXmlQuery query;
	QFileInfo fileInfo;
	
	QPlexyMimePrivate()
	{
		newQuery = QString("declare namespace ns = 'http://www.freedesktop.org/standards/shared-mime-info';\ndeclare variable $internalFile external;\n");
	}
	
	void evaluate(QString &result)
	{
		if(!query.isValid())
			return;
		
		query.evaluateTo(&result);
	}
	
	void evaluate(QStringList &result)
	{
		if(!query.isValid())
			return;
		
		query.evaluateTo(&result);
	}

	void setQuery(QString tmpQuery)
	{
		QString tmp = newQuery + tmpQuery;
		query.setQuery(newQuery + tmpQuery);
	
		if(!query.isValid())
		{
			qDebug() << "Invalid query" << tmpQuery;
			return;
		}
	}
	
	bool setGenericMime(const QString &mimeType)
	{
		if(mimeType.isEmpty())
			return false;
	
		QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type[@type='%1']/ns:glob/@pattern/string()").arg(mimeType);
		
		setQuery(tmpQuery);
	
		QString result;
		evaluate(result);
		
		if(result.isEmpty())
			return false;
		
		QString extension = result.remove(0,1);
		QString filename = QString("test") + extension;
		
		fileInfo.setFile(filename.simplified());
		
		return true;
	}
};

QPlexyMime::QPlexyMime (QObject *parent)
	: d(new QPlexyMimePrivate)
{
	QFile sourceDocument(":/freedesktop.org.xml");
	sourceDocument.open(QIODevice::ReadOnly);
	
	d->output = sourceDocument.readAll();

    d->outputBuffer.setBuffer(&d->output);
    d->outputBuffer.open(QIODevice::ReadOnly);
	d->query.bindVariable("internalFile", &d->outputBuffer);
}

QPlexyMime::~QPlexyMime()
{
	delete d;
}

QString QPlexyMime::fromFileName (const QString& fileName)
{
	d->fileInfo.setFile(fileName);
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../@type/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}
/*
QString QPlexyMime::fromFile (const QString& fileName)
{
	QFileInfo fileInfo(fileName);
    if (fileInfo.isDir())
	{
		return("inode/directory");
	}

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
		return(QString());

	QString mimeType = fromFile(&file);

    file.close();

    return(mimeType);
}

QString QPlexyMime::fromFile (QFile *file)
{
	return QString();
}
*/

QString QPlexyMime::genericIconNameMime (const QString& mimeType)
{
	if(d->setGenericMime(mimeType))
	{
		return genericIconName();
	}

	return QString();
}

QString QPlexyMime::expandedAcronymMime (const QString& mimeType)
{
	if(d->setGenericMime(mimeType))
		return expandedAcronym();

	return QString();
}

QString QPlexyMime::descriptionMime (const QString& mimeType, const QString &lang)
{
	if(d->setGenericMime(mimeType))
		if(lang.isEmpty())
			return description();
		else
			return description(lang);

	return QString();
}

QString QPlexyMime::subClassOfMime (const QString& mimeType)
{
	if(d->setGenericMime(mimeType))
		return subClassOf();

	return QString();
}

QString QPlexyMime::acronymMime (const QString& mimeType)
{
	if(d->setGenericMime(mimeType))
		return acronym();

	return QString();
}

QString QPlexyMime::aliasMime (const QString& mimeType)
{
	if(d->setGenericMime(mimeType))
		return alias();

	return QString();
}

QString QPlexyMime::genericIconName (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:generic-icon/@name/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

QString QPlexyMime::expandedAcronym (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:expanded-acronym/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

QString QPlexyMime::description (const QString &lang) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery;
	if(lang.isEmpty())
		tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:comment/string()").arg(ext);
	else
		tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:comment[@xml:lang='%2']/string()").arg(ext).arg(lang);

	d->setQuery(tmpQuery);
	
	QString retValue;
	
	if(lang.isEmpty())
	{
		QStringList result;
		d->evaluate(result);
		if(!result.isEmpty())
			retValue = result.first();
	}
	else
	{
		QString result;
		d->evaluate(result);
		
		retValue = result;
	}
	
	return retValue.simplified();
}

QString QPlexyMime::subClassOf (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:sub-class-of/@type/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

QString QPlexyMime::mimeType (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../@type/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

QString QPlexyMime::acronym (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:acronym/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

QString QPlexyMime::alias (void) const
{
	QString ext = d->fileInfo.suffix();
	QString tmpQuery = QString("doc($internalFile)/ns:mime-info/ns:mime-type/ns:glob[@pattern='*.%1']/../ns:alias/@type/string()").arg(ext);
	
	d->setQuery(tmpQuery);
	
	QString result;
	d->evaluate(result);
	
	return result.simplified();
}

