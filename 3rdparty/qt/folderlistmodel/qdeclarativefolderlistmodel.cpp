/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QFileSystemModel>
#include <QDesktopServices>
#include <QCryptographicHash>
#include <QDebug>
#include <qdeclarativecontext.h>

#include "qdeclarativefolderlistmodel.h"
#include <freedesktopmime.h>
#include <plexyqmlglue.h>

#ifndef QT_NO_DIRMODEL


class QDeclarativeFolderListModelPrivate
{
public:
    QDeclarativeFolderListModelPrivate()
        : sortField(QDeclarativeFolderListModel::Name), sortReversed(false), count(0) {
        nameFilters << QLatin1String("*");
    }

    // TODO: Sorting workaround fix
    void updateSorting() {
        QDir::SortFlags flags = 0;
        switch(sortField) {
        case QDeclarativeFolderListModel::Unsorted:
            flags |= QDir::Unsorted;
            break;
        case QDeclarativeFolderListModel::Name:
            flags |= QDir::Name;
            break;
        case QDeclarativeFolderListModel::Time:
            flags |= QDir::Time;
            break;
        case QDeclarativeFolderListModel::Size:
            flags |= QDir::Size;
            break;
        case QDeclarativeFolderListModel::Type:
            flags |= QDir::Type;
            break;
        }

        if (sortReversed)
            flags |= QDir::Reversed;

//        model.setSorting(flags);
    }

    QFileSystemModel model;
    QUrl folder;
    QStringList nameFilters;
    QModelIndex folderIndex;
    QDeclarativeFolderListModel::SortField sortField;
    bool sortReversed;
    int count;
};

/*!
    \qmlclass FolderListModel QDeclarativeFolderListModel
    \ingroup qml-working-with-data
    \brief The FolderListModel provides a model of the contents of a file system folder.

    FolderListModel provides access to information about the contents of a folder
    in the local file system, exposing a list of files to views and other data components.

    \note This type is made available by importing the \c PlexyDesk.FolderListModel module.
    \e{Elements in the PlexyDesk module are not guaranteed to remain compatible
    in future versions.}

    \bold{import PlexyDesk.FolderListModel 1.0}

    The \l folder property specifies the folder to access. Information about the
    files and directories in the folder is supplied via the model's interface.
    Components access names and paths via the following roles:

    \list
    \o fileName
    \o filePath
    \o fileIcon
    \o fileMimeIcon
    \o fileTypeIcon
    \endlist

    Additionally a file entry can be differentiated from a folder entry via the
    isFolder() method.

    \section1 Filtering

    Various properties can be set to filter the number of files and directories
    exposed by the model.

    The \l nameFilters property can be set to contain a list of wildcard filters
    that are applied to names of files and directories, causing only those that
    match the filters to be exposed.

    Directories can be included or excluded using the \l showDirs property, and
    navigation directories can also be excluded by setting the \l showDotAndDotDot
    property to false.

    It is sometimes useful to limit the files and directories exposed to those
    that the user can access. The \l showOnlyReadable property can be set to
    enable this feature.

    \section1 Example Usage

    The following example shows a FolderListModel being used to provide a list
    of QML files in a \l ListView:

    \snippet doc/src/snippets/declarative/folderlistmodel.qml 0

    \section1 Path Separators

    Qt uses "/" as a universal directory separator in the same way that "/" is
    used as a path separator in URLs. If you always use "/" as a directory
    separator, Qt will translate your paths to conform to the underlying
    operating system.

    \sa {QML Data Models}
*/

QDeclarativeFolderListModel::QDeclarativeFolderListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mImageCache = dynamic_cast<PlexyDesk::ImageCache *>(PlexyDesk::PlexyQmlGlue::qmlEngine()->imageProvider("plexydesk"));

    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[FileIconRole] = "fileIcon";
    roles[FileMimeIconRole] = "fileMimeIcon";
    roles[FileTypeIconRole] = "fileTypeIcon";
    setRoleNames(roles);

    d = new QDeclarativeFolderListModelPrivate;
    d->model.setFilter(QDir::AllDirs | QDir::Files | QDir::Drives | QDir::NoDotAndDotDot);
    connect(&d->model, SIGNAL(rowsInserted(const QModelIndex&,int,int))
            , this, SLOT(inserted(const QModelIndex&,int,int)));
    connect(&d->model, SIGNAL(rowsRemoved(const QModelIndex&,int,int))
            , this, SLOT(removed(const QModelIndex&,int,int)));
    connect(&d->model, SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&))
            , this, SLOT(handleDataChanged(const QModelIndex&,const QModelIndex&)));
    connect(&d->model, SIGNAL(modelReset()), this, SLOT(refresh()));
    connect(&d->model, SIGNAL(layoutChanged()), this, SLOT(refresh()));


    SetToHome();
}

QDeclarativeFolderListModel::~QDeclarativeFolderListModel()
{
    delete d;
}

void QDeclarativeFolderListModel::SetToHome()
{
    QString path;

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    path = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
#elif defined(Q_OS_WIN)
    path = "/" + QDesktopServices::storageLocation(QDesktopServices::DesktopLocation).replace(QString("\\"),QString("/"));
#endif

    setFolder(QUrl(path));
}

QVariant QDeclarativeFolderListModel::getMimeTypeImagePath(const QString file) const
{
    QString fileLocal = file;

    QString filenameMD5 = QString(QCryptographicHash::hash(fileLocal.toUtf8(), QCryptographicHash::Md5)
                                        .toHex().toUpper());

    if (mImageCache->isCached(filenameMD5))
        return QVariant("image://plexydesk/" + filenameMD5);

    QFreeDesktopMime mime;
    QString mimeType = mime.fromFile(fileLocal);

//    qDebug() << "Requested mime info for: " << fileLocal << " -> " << mimeType;

    QString imgfile;
    QString theme = PlexyDesk::Config::getInstance()->themepackName();

    if (mimeType.isNull())
        mimeType = "";

    if (mimeType.contains("inode/directory", Qt::CaseInsensitive))
        imgfile = "folder.png";

    /* Executables */
    else if (mimeType.contains("ms-dos", Qt::CaseInsensitive))
        imgfile = "exe.png";
    else if (mimeType.contains("x-executable", Qt::CaseInsensitive))
        imgfile = "obj.png";
    else if (mimeType.contains("sharedlib", Qt::CaseInsensitive))
        imgfile = "so.png";
    else if (mimeType.contains("library-la", Qt::CaseInsensitive))
        imgfile = "la.png";
    else if(mimeType.contains("shellscript", Qt::CaseInsensitive))
        imgfile = "script.png";
    else if(mimeType.contains("x-desktop", Qt::CaseInsensitive))
        imgfile = "exec.png";

    /* Sources */
    else if (mimeType.contains("x-c++src", Qt::CaseInsensitive))
        imgfile = "cpp.png";
    else if (mimeType.contains("x-csrc", Qt::CaseInsensitive))
        imgfile = "c.png";
    else if (mimeType.contains("x-chdr", Qt::CaseInsensitive))
        imgfile = "h.png";
    else if (mimeType.contains("x-object", Qt::CaseInsensitive))
        imgfile = "obj.png";

    /* Int. Lang. */
    else if (mimeType.contains("x-php", Qt::CaseInsensitive))
        imgfile = "php.png";
    else if (mimeType.contains("x-perl", Qt::CaseInsensitive))
        imgfile = "pl.png";
    else if (mimeType.contains("x-python", Qt::CaseInsensitive))
        imgfile = "py.png";
    else if (mimeType.contains("x-java", Qt::CaseInsensitive))
        imgfile = "java.png";
    else if (mimeType.contains("javascript", Qt::CaseInsensitive))
        imgfile = "js.png";
    else if (mimeType.contains("x-ruby", Qt::CaseInsensitive))
        imgfile = "rb.png";
    else if (mimeType.contains("application/xml", Qt::CaseInsensitive))
        imgfile = "xml.png";

    /* Documents */
    else if(mimeType.contains("pdf", Qt::CaseInsensitive))
        imgfile = "pdf.png";
    else if(mimeType.contains("text/plain", Qt::CaseInsensitive))
        imgfile = "txt.png";
    else if(mimeType.contains(QRegExp("rtf|msword|wordprocessing|opendocument.text", Qt::CaseInsensitive)))
        imgfile = "doc.png";
    else if(mimeType.contains(QRegExp("ms-excel|spreadsheet", Qt::CaseInsensitive)))
        imgfile = "xls.png";
    else if(mimeType.contains(QRegExp("ms-powerpoint|presentation", Qt::CaseInsensitive)))
        imgfile = "ppt.png";
    else if(mimeType.contains("database", Qt::CaseInsensitive))
        imgfile = "odb.png";
    else if (mimeType.contains("html", Qt::CaseInsensitive))
        imgfile = "html.png";
    else if (mimeType.contains("x-uri", Qt::CaseInsensitive))
        imgfile = "url.png";

    /* Images */
    else if (mimeType.contains("image/svg", Qt::CaseInsensitive))
        imgfile = "svg.png";
    else if (mimeType.contains("image/", Qt::CaseInsensitive))
        imgfile = "img.png";

    /* Archives */
    else if (mimeType.contains(QRegExp("ms-cab|stuffit|/zip|x-(compressed|(r|t)ar|7z|(g|b)zip)", Qt::CaseInsensitive)))
        imgfile = "archive.png";

    /* Video */
    else if (mimeType.contains("video/", Qt::CaseInsensitive))
        imgfile = "video.png";

    /* Audio */
    else if (mimeType.contains("audio/", Qt::CaseInsensitive))
        imgfile = "audio.png";

    /* Disc Images */
    else if (mimeType.contains("cd-image", Qt::CaseInsensitive))
        imgfile = "iso.png";

    /* Packages */
    else if (mimeType.contains(QRegExp("x-(apple-diskimage|rpm|deb|msi)", Qt::CaseInsensitive)))
        imgfile = "package.png";

    /* Misc */
    else if (mimeType.contains("bittorrent", Qt::CaseInsensitive))
        imgfile = "torrent.png";

    /* Unknown */
    else
        imgfile = "unknown.png";

    mImageCache->addToCached(imgfile, filenameMD5, theme);

    return QVariant("image://plexydesk/" + filenameMD5);
}

QVariant QDeclarativeFolderListModel::getFileTypeImagePath(const QString file) const
{
    QString fileType = file.toLower().section(" ",0,0);

    QString imgfile;

    // On win folders are reported as "File Folder", on Lin as "Folder"
    if (file.contains("folder", Qt::CaseInsensitive))
        imgfile = "folder";
    else if (file.contains(QRegExp("drive|device", Qt::CaseInsensitive)))
        imgfile = "hdd";

    /* Executables */
    else if (fileType.compare("exe") == 0)
        imgfile = "exe";
    else if(fileType.contains(QRegExp("^(sh|bat)")))
        imgfile = "script";
    else if (fileType.compare("lnk") == 0)
        imgfile = "lnk";
    else if(fileType.compare("desktop") == 0)
        imgfile = "exec";
    else if(fileType.compare("la") == 0)
        imgfile = "la";
    else if (fileType.contains(QRegExp("^(so|dll)")))
        imgfile = "so";

    /* Sources */
    else if (fileType.compare("cpp") == 0)
        imgfile = "cpp";
    else if (fileType.compare("c") == 0)
        imgfile = "c";
    else if (fileType.compare("h") == 0)
        imgfile = "h";
    else if (fileType.compare("o") == 0)
        imgfile = "obj";
    else if (fileType.contains(QRegExp("^(obj|out)")))
        imgfile = "obj";

    /* Int. Lang. */
    else if (fileType.contains(QRegExp("^(php|phtml)")))
        imgfile = "php";
    else if (fileType.compare("pl") == 0)
        imgfile = "pl";
    else if (fileType.contains(QRegExp("^(py|pyc)$")))
        imgfile = "py";
    else if (fileType.contains(QRegExp("^(java|jar|jnlp)")))
        imgfile = "java";
    else if (fileType.compare("js") == 0)
        imgfile = "js";
    else if (fileType.compare("rb") == 0)
        imgfile = "rb";
    else if (fileType.compare("xml") == 0)
        imgfile = "xml";

    /* Documents */
    else if(fileType.compare("pdf") == 0)
        imgfile = "pdf";
    else if(fileType.compare("txt") == 0)
        imgfile = "txt";
    else if(fileType.contains(QRegExp("^(rtf|doc|odt)")))
        imgfile = "doc";
    else if(fileType.contains(QRegExp("^(xlt|xls|xlb|ods)")))
        imgfile = "xls";
    else if(fileType.contains(QRegExp("^(ppt|pps|odp)")))
        imgfile = "ppt";
    else if(fileType.contains(QRegExp("^(db|odb)")))
        imgfile = "odb";
    else if (fileType.contains(QRegExp("^(htm|shtmlhtml)")))
        imgfile = "html";
    else if (fileType.compare("url") == 0)
        imgfile = "url";

    /* Images */
    else if (fileType.compare("svg") == 0)
        imgfile = "svg";
    else if (fileType.contains(QRegExp("^(png|jpeg|jpg|bmp|tiff)")))
        imgfile = "img";

    /* Archives */
    else if (fileType.contains(QRegExp("^(zip|rar|7z|gz|bz2|tar|cab|sit)")))
        imgfile = "archive";

    /* Video */
    else if (fileType.contains(QRegExp("^(mpeg|mpg|avi|mp4|m4v|mkv|wmv|mov)")))
        imgfile = "video";

    /* Audio */
    else if (fileType.contains(QRegExp("^(aiff|mp3|wma|flac|m3u)")))
        imgfile = "audio";

    /* Disc Images */
    else if (fileType.contains(QRegExp("^(iso|bin|nrg|daa)")))
        imgfile = "iso";

    /* Packages */
    else if (fileType.contains(QRegExp("^(rpm|deb|dmg|tgz|ebuild|msi)")))
        imgfile = "package";

    /* Misc */
    else if (fileType.compare("torrent") == 0)
        imgfile = "torrent";

    /* Unknown */
    else
        imgfile = "unknown";

//    qDebug() << "Requested icon for: " << file << " (" << fileType << ") --> image://plexydesk/" << imgfile;

    return QVariant("image://plexydesk/" + imgfile);
}

bool QDeclarativeFolderListModel::openFile(QString file)
{
    QString fileLocal = QUrl(file).toLocalFile();
    QFileInfo info(fileLocal);
    if (info.isDir())
       return false;

    return QDesktopServices::openUrl(QUrl(file,QUrl::TolerantMode));
}

QVariant QDeclarativeFolderListModel::data(const QModelIndex &index, int role) const
{
    QVariant rv;
    QModelIndex modelIndex = d->model.index(index.row(), 0, d->folderIndex);
    if (modelIndex.isValid()) {
        if (role == FileNameRole)
            rv = d->model.data(modelIndex, QFileSystemModel::FileNameRole).toString();
        else if (role == FilePathRole)
            rv = QUrl::fromLocalFile(d->model.data(modelIndex, QFileSystemModel::FilePathRole).toString());
        else if (role == FileIconRole)
            rv = d->model.data(modelIndex, QFileSystemModel::FileIconRole);
        else if (role == FileMimeIconRole)
            rv = getMimeTypeImagePath(d->model.data(modelIndex, QFileSystemModel::FilePathRole).toString());
        else if (role == FileTypeIconRole)
            rv = getFileTypeImagePath(d->model.type(modelIndex));
    }
    return rv;
}

/*!
    \qmlproperty int FolderListModel::count

    Returns the number of items in the current folder that match the
    filter criteria.
*/
int QDeclarativeFolderListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->count;
}

/*!
    \qmlproperty string FolderListModel::folder

    The \a folder property holds a URL for the folder that the model is
    currently providing.

    The value is a URL expressed as a string, and must be a \c file: or \c qrc:
    URL, or a relative URL.

    By default, the value is an invalid URL.
*/
QUrl QDeclarativeFolderListModel::folder() const
{
    return d->folder;
}

void QDeclarativeFolderListModel::setFolder(const QUrl &folder)
{
    if (folder == d->folder)
        return;
    QModelIndex index = d->model.index(folder.toLocalFile());
    if ((index.isValid() && d->model.isDir(index)) || folder.toLocalFile().isEmpty()) {
        d->folder = folder;
        QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
        emit folderChanged();
        d->model.setRootPath(folder.toLocalFile());
    }
}

/*!
    \qmlproperty url FolderListModel::parentFolder

    Returns the URL of the parent of of the current \l folder.
*/
QUrl QDeclarativeFolderListModel::parentFolder() const
{
    QString localFile = d->folder.toLocalFile();
    if (!localFile.isEmpty()) {
        QDir dir(localFile);
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WIN)
        if (dir.isRoot())
            dir.setPath("");
        else
#endif
            dir.cdUp();
        localFile = dir.path();
    } else {
        int pos = d->folder.path().lastIndexOf(QLatin1Char('/'));
        if (pos == -1)
            return QUrl();
        localFile = d->folder.path().left(pos);
    }
    return QUrl::fromLocalFile(localFile);
}

/*!
    \qmlproperty list<string> FolderListModel::nameFilters

    The \a nameFilters property contains a list of file name filters.
    The filters may include the ? and * wildcards.

    The example below filters on PNG and JPEG files:

    \qml
    FolderListModel {
        nameFilters: [ "*.png", "*.jpg" ]
    }
    \endqml

    \note Directories are not excluded by filters.
*/
QStringList QDeclarativeFolderListModel::nameFilters() const
{
    return d->nameFilters;
}

void QDeclarativeFolderListModel::setNameFilters(const QStringList &filters)
{
    d->nameFilters = filters;
    d->model.setNameFilters(d->nameFilters);
}

void QDeclarativeFolderListModel::classBegin()
{
}

void QDeclarativeFolderListModel::componentComplete()
{
    if (!d->folder.isValid() || d->folder.toLocalFile().isEmpty() || !QDir().exists(d->folder.toLocalFile()))
        setFolder(QUrl(QLatin1String("file://")+QDir::currentPath()));

    if (!d->folderIndex.isValid())
        QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
}

/*!
    \qmlproperty enumeration FolderListModel::sortField

    The \a sortField property contains field to use for sorting.  sortField
    may be one of:
    \list
    \o Unsorted - no sorting is applied.  The order is system default.
    \o Name - sort by filename
    \o Time - sort by time modified
    \o Size - sort by file size
    \o Type - sort by file type (extension)
    \endlist

    \sa sortReversed
*/
QDeclarativeFolderListModel::SortField QDeclarativeFolderListModel::sortField() const
{
    return d->sortField;
}

void QDeclarativeFolderListModel::setSortField(SortField field)
{
    if (field != d->sortField) {
        d->sortField = field;
        d->updateSorting();
    }
}

/*!
    \qmlproperty bool FolderListModel::sortReversed

    If set to true, reverses the sort order.  The default is false.

    \sa sortField
*/
bool QDeclarativeFolderListModel::sortReversed() const
{
    return d->sortReversed;
}

void QDeclarativeFolderListModel::setSortReversed(bool rev)
{
    if (rev != d->sortReversed) {
        d->sortReversed = rev;
        d->updateSorting();
    }
}

/*!
    \qmlmethod bool FolderListModel::isFolder(int index)

    Returns true if the entry \a index is a folder; otherwise
    returns false.
*/
bool QDeclarativeFolderListModel::isFolder(int index) const
{
    if (index != -1) {
        QModelIndex idx = d->model.index(index, 0, d->folderIndex);
        if (idx.isValid())
            return d->model.isDir(idx);
    }
    return false;
}

void QDeclarativeFolderListModel::refresh()
{
    d->folderIndex = QModelIndex();
    if (d->count) {
        emit beginRemoveRows(QModelIndex(), 0, d->count);
        d->count = 0;
        emit endRemoveRows();
    }

    d->folderIndex = d->model.index(d->folder.toLocalFile());
    int newcount = d->model.rowCount(d->folderIndex);
    if (newcount) {
        emit beginInsertRows(QModelIndex(), 0, newcount-1);
        d->count = newcount;
        emit endInsertRows();
    }
}

void QDeclarativeFolderListModel::inserted(const QModelIndex &index, int start, int end)
{
    if (index == d->folderIndex) {
        emit beginInsertRows(QModelIndex(), start, end);
        d->count = d->model.rowCount(d->folderIndex);
        emit endInsertRows();
        // TODO: Add thread to cache each added index to IconCache
    }
}

void QDeclarativeFolderListModel::removed(const QModelIndex &index, int start, int end)
{
    if (index == d->folderIndex) {
        emit beginRemoveRows(QModelIndex(), start, end);
        d->count = d->model.rowCount(d->folderIndex);
        emit endRemoveRows();
    }
}

void QDeclarativeFolderListModel::handleDataChanged(const QModelIndex &start, const QModelIndex &end)
{
    if (start.parent() == d->folderIndex)
        emit dataChanged(index(start.row(),0), index(end.row(),0));
}

/*!
    \qmlproperty bool FolderListModel::showDirs

    If true, directories are included in the model; otherwise only files
    are included.

    By default, this property is true.

    Note that the nameFilters are not applied to directories.

    \sa showDotAndDotDot
*/
bool QDeclarativeFolderListModel::showDirs() const
{
    return d->model.filter() & QDir::AllDirs;
}

void  QDeclarativeFolderListModel::setShowDirs(bool on)
{
    if (!(d->model.filter() & QDir::AllDirs) == !on)
        return;
    if (on)
        d->model.setFilter(d->model.filter() | QDir::AllDirs | QDir::Drives);
    else
        d->model.setFilter(d->model.filter() & ~(QDir::AllDirs | QDir::Drives));
}

/*!
    \qmlproperty bool FolderListModel::showDotAndDotDot

    If true, the "." and ".." directories are included in the model; otherwise
    they are excluded.

    By default, this property is false.

    \sa showDirs
*/
bool QDeclarativeFolderListModel::showDotAndDotDot() const
{
    return !(d->model.filter() & QDir::NoDotAndDotDot);
}

void  QDeclarativeFolderListModel::setShowDotAndDotDot(bool on)
{
    if (!(d->model.filter() & QDir::NoDotAndDotDot) == on)
        return;
    if (on)
        d->model.setFilter(d->model.filter() & ~QDir::NoDotAndDotDot);
    else
        d->model.setFilter(d->model.filter() | QDir::NoDotAndDotDot);
}

/*!
    \qmlproperty bool FolderListModel::showOnlyReadable

    If true, only readable files and directories are shown; otherwise all files
    and directories are shown.

    By default, this property is false.

    \sa showDirs
*/
bool QDeclarativeFolderListModel::showOnlyReadable() const
{
    return d->model.filter() & QDir::Readable;
}

void QDeclarativeFolderListModel::setShowOnlyReadable(bool on)
{
    if (!(d->model.filter() & QDir::Readable) == !on)
        return;
    if (on)
        d->model.setFilter(d->model.filter() | QDir::Readable);
    else
        d->model.setFilter(d->model.filter() & ~QDir::Readable);
}


#endif // QT_NO_DIRMODEL
