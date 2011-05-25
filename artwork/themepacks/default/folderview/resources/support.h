/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
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

#ifndef FOLDERSUPPORT_H
#define FOLDERSUPPORT_H
#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeParserStatus>

QT_MODULE(Declarative)

class QDeclarativeContext;

class Q_DECLARATIVE_EXPORT Support : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
public:
    Support(QObject *parent = 0);
    ~Support();
    Q_PROPERTY(QString home READ home);
    QString home();
    virtual void classBegin(){}
    virtual void componentComplete(){}
};

QML_DECLARE_TYPE(Support)

#endif
