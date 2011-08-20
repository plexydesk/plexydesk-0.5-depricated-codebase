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

#ifndef TESTMIME_H
#define TESTMIME_H

#include <QObject>
#include <QDirIterator>

#include <qplexymime.h>


class TestMime : public QObject
{
    Q_OBJECT

public:
    TestMime(QObject *parent = 0);
    ~TestMime();

Q_SIGNALS:
    void closeApplication();

public slots:
    void startTest();
    void cannotFound(const QString, const QString);
    void fromFileNameMime(const MimePairType);
    void genericIconNameMime(const MimePairType);
    void expandedAcronymMime(const MimePairType);
    void descriptionWithLang(const MimeWithLangType);
    void descriptionWithList(const MimeWithListType);
    void subclassMime(const MimePairType);
    void acronymMime(const MimePairType);
    void aliasMime(const MimePairType);

private:
    QPlexyMime *mMime;
    QDirIterator *dirIterator;
};

#endif // TESTMIME_H
