/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Dariusz Mikulski <dariusz.mikulski@gmail.com>
*  Date: 02.05.2010
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

#include <QTimer>
#include <QCoreApplication>

TestMime::TestMime(QObject *parent)
    : QObject(parent)
{
    QTimer::singleShot(1000, this, SLOT(startTest()));
}

void TestMime::startTest()
{
    qDebug() << "Start TEST";

    mMime = new QPlexyMime(this);
    connect(mMime, SIGNAL(fromFileNameMime(const MimePairType)), this, SLOT(fromFileNameMime(const MimePairType)));
    connect(mMime, SIGNAL(genericIconNameMime(const MimePairType)), this, SLOT(genericIconNameMime(const MimePairType)));
    connect(mMime, SIGNAL(expandedAcronymMime(const MimePairType)), this, SLOT(expandedAcronymMime(const MimePairType)));
    connect(mMime, SIGNAL(cannotFound(const QString, const QString)), this, SLOT(cannotFound(const QString, const QString)));
    connect(mMime, SIGNAL(descriptionWithLang(const MimeWithLangType)), this, SLOT(descriptionWithLang(const MimeWithLangType)));
    connect(mMime, SIGNAL(descriptionWithList(const MimeWithListType)), this, SLOT(descriptionWithList(const MimeWithListType)));
    connect(mMime, SIGNAL(subclassMime(const MimePairType)), this, SLOT(subclassMime(const MimePairType)));
    connect(mMime, SIGNAL(acronymMime(const MimePairType)), this, SLOT(acronymMime(const MimePairType)));
    connect(mMime, SIGNAL(aliasMime(const MimePairType)), this, SLOT(aliasMime(const MimePairType)));

    // set dir iterator
    dirIterator = new QDirIterator("/var", QStringList("*.*"), QDir::AllDirs|QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while(dirIterator->hasNext())
    {
            // make sure to call next, failing todo so will result in infinite loop
            dirIterator->next();

            // found required file
            if(!dirIterator->fileInfo().suffix().isEmpty())
            {
                mMime->fromFileName(dirIterator->fileInfo().fileName());
            }
    }

    for(int i=0; i<100; i++) {
    mMime->fromFileName("test.ez");
    mMime->fromFileName("test.exe");
    mMime->fromFileName("test.xml");
    mMime->fromFileName("test.cpp");
    mMime->fromFileName("test.la");
    mMime->fromFileName("test.so");
    mMime->fromFileName("test.txt");
    mMime->fromFileName("test.db");
    mMime->fromFileName("test.log");
    mMime->fromFileName("test.lnk");
    mMime->fromFileName("test.ld");
    mMime->fromFileName("test.sh");
    mMime->fromFileName("test.tiff");
    mMime->fromFileName("test.png");
    mMime->fromFileName("test.bmp");
    mMime->fromFileName("test.pdf");
}

}

void TestMime::cannotFound(const QString error, const QString str)
{
    qDebug() << error << str;
}

void TestMime::fromFileNameMime(const MimePairType mimeType)
{
    if(mimeType.second.isEmpty())
        return;

    qDebug() << "fromFileNameMime()" << mimeType;

    mMime->genericIconName(mimeType.second);
    mMime->expandedAcronym(mimeType.second);
//    mMime->description(mimeType.second);
    mMime->description(mimeType.second, "pl");
    mMime->subclassOfMime(mimeType.second);
    mMime->acronym(mimeType.second);
    mMime->alias(mimeType.second);
}

void TestMime::genericIconNameMime(const MimePairType mimeType)
{
    qDebug() << "genericIconNameMime()" << mimeType;
}

void TestMime::expandedAcronymMime(const MimePairType mimeType)
{
    qDebug() << "expandedAcronymMime()" << mimeType;
}

void TestMime::descriptionWithLang(const MimeWithLangType mimeWithLang)
{
    qDebug() << "descriptionWithLang()" << mimeWithLang;
}

void TestMime::descriptionWithList(const MimeWithListType mimeWithList)
{
    qDebug() << "descriptionWithList()" << mimeWithList;
}

void TestMime::subclassMime(const MimePairType mimeType)
{
    qDebug() << "subclassMime()" << mimeType;
}

void TestMime::acronymMime(const MimePairType mimeType)
{
    qDebug() << "acronymMime()" << mimeType;
}

void TestMime::aliasMime(const MimePairType mimeType)
{
    qDebug() << "aliasMime()" << mimeType;
}
