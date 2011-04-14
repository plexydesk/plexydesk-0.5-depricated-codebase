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


#ifndef QPLEXMIME_H
#define QPLEXMIME_H

#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QFutureWatcher>
#include <QFileInfo>
#include <QXmlQuery>
#include <QMutex>
#include <QCache>
#include <QStringList>
#include <plexyconfig.h>

typedef QPair<QString, QString> MimePairType;
typedef QPair<MimePairType, QString> MimeWithLangType;
typedef QPair<QString, QStringList> MimeWithListType;

#if defined(_WIN32)
  #if defined(plexymime_EXPORTS)
    #define  QPLEXYMIME_EXPORT __declspec(dllexport)
  #else
    #define  QPLEXYMIME_EXPORT __declspec(dllimport)
  #endif
#else
    #ifdef __SYSTEM_HAVE_GCC_VISIBILITY
        #define HIDDEN_SYM __attribute__ ((visibility("hidden")))
        #define VISIBLE_SYM __attribute__ ((visibility("default")))
        #define QPLEXYMIME_EXPORT __attribute__ ((visibility("default")))
    #endif
#endif

class QPLEXYMIME_EXPORT QPlexyMime : public QObject
{
    Q_OBJECT
public:
    QPlexyMime(QObject *parent = 0);
    ~QPlexyMime();

    void fromFileName(const QString &fileName);
    void genericIconName(const QString &mimeType);
    void expandedAcronym(const QString &mimeType);
    void description(const QString &mimeType, const QString &lang = QString());
    void subclassOfMime(const QString &mimeType);
    void acronym(const QString &mimeType);
    void alias(const QString &mimeType);

signals:
    void cannotFound(const QString errorName, const QString);
    void cannotFound(const QString errorName, const MimePairType);
    void fromFileNameMime(const MimePairType);
    void genericIconNameMime(const MimePairType);
    void expandedAcronymMime(const MimePairType);
    void descriptionWithLang(const MimeWithLangType);
    void descriptionWithList(const MimeWithListType);
    void subclassMime(const MimePairType);
    void acronymMime(const MimePairType);
    void aliasMime(const MimePairType);

private slots:
    void internalFromFileName(const QString &);
    void internalGenericIconName(const QString &);
    void internalExpandedAcronym(const QString &);
    void internalDescription(const QString &, const QString &);
    void internalSubclassOfMime(const QString &);
    void internalAcronym(const QString &);
    void internalAlias(const QString &mimeType);

private:
    bool getGenericMime(const QString &mimeType, QFileInfo *);
    QString getInternalQuery() {
        return mInternalQuery;
    }
    QByteArray getInternalOutput() {
        return mInternalOutput;
    }
    QString evaluate(const QString &);
    QStringList evaluateToList(const QString &);

    QString mInternalQuery;
    QByteArray mInternalOutput;
    QMutex *mMutex;
    QCache<QString, QString> *mFileNameMimeCache;
    QCache<QString, QString> *mGenericExtMimeCache;
    QCache<QString, QString> *mGenericMimeIconNameCache;
    QCache<QString, QString> *mExpandedAcronymMimeAcronymCache;
    QCache<MimePairType, QString> *mDescriptionCacheWithLang;
    QCache<QString, QStringList> *mDescriptionCache;
    QCache<QString, QString> *mSubclassOfMimeCache;
    QCache<QString, QString> *mAcronymMimeCache;
    QCache<QString, QString> *mAliasMimeCache;
};

#endif // QPLEXMIME_H
