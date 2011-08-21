/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
*                 PhobosK <phobosk@kbfx.net>
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

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QCryptographicHash>

#include "support.h"
#include <freedesktopmime.h>
#include <plexyconfig.h>


Support::Support(QObject *parent) : QObject(parent)
{
    QDeclarativeContext *context = PlexyDesk::Config::qmlEngine()->rootContext();
    context->setContextProperty("Mime", this);
    mImageCache = dynamic_cast<PlexyDesk::ImageCache *>(PlexyDesk::Config::qmlEngine()->imageProvider("plexydesk"));
}

Support::~Support(){}

QString Support::home()
{
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    return QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
#elif defined(Q_OS_WIN)
    return "/" + QDesktopServices::storageLocation(QDesktopServices::DesktopLocation).replace(QString("\\"),QString("/"));
#endif
}

QString Support::setMimeType(QString file)
{
    QString fileLocal = QUrl(file).toLocalFile();

    QString filenameMD5 = QString(QCryptographicHash::hash(fileLocal.toUtf8(), QCryptographicHash::Md5)
                                        .toHex().toUpper());

    if (mImageCache->isCached(filenameMD5))
        return ("image://plexydesk/" + filenameMD5);

    QFreeDesktopMime mime;
    QString mimeType = mime.fromFile(fileLocal);

//    qDebug() << "Requested mime info for: " << fileLocal << " -> " << mimeType;

    setMimeTypeImage(mimeType, filenameMD5);

    return ("image://plexydesk/" + filenameMD5);
}

void Support::setMimeTypeImage(QString mimetype, QString filenamemd5)
{
    QString imgfile;
    QString theme = "default";

    if (mimetype.isNull())
        mimetype = "";

    if (mimetype.contains("directory", Qt::CaseInsensitive))
        imgfile = "folder.png";

    /* Executables */
    else if (mimetype.contains("ms-dos", Qt::CaseInsensitive))
        imgfile = "exe.png";
    else if(mimetype.contains("shellscript", Qt::CaseInsensitive))
        imgfile = "script.png";
    else if(mimetype.contains("x-desktop", Qt::CaseInsensitive))
        imgfile = "exec.png";

    /* Sources */
    else if (mimetype.contains("x-c++src", Qt::CaseInsensitive))
        imgfile = "cpp.png";
    else if (mimetype.contains("x-csrc", Qt::CaseInsensitive))
        imgfile = "c.png";
    else if (mimetype.contains("x-chdr", Qt::CaseInsensitive))
        imgfile = "h.png";
    else if (mimetype.contains("x-object", Qt::CaseInsensitive))
        imgfile = "obj.png";

    /* Int. Lang. */
    else if (mimetype.contains("x-php", Qt::CaseInsensitive))
        imgfile = "php.png";
    else if (mimetype.contains("x-perl", Qt::CaseInsensitive))
        imgfile = "pl.png";
    else if (mimetype.contains("x-python", Qt::CaseInsensitive))
        imgfile = "py.png";
    else if (mimetype.contains("x-java", Qt::CaseInsensitive))
        imgfile = "java.png";
    else if (mimetype.contains("javascript", Qt::CaseInsensitive))
        imgfile = "js.png";
    else if (mimetype.contains("x-ruby", Qt::CaseInsensitive))
        imgfile = "rb.png";
    else if (mimetype.contains("application/xml", Qt::CaseInsensitive))
        imgfile = "xml.png";

    /* Documents */
    else if(mimetype.contains("pdf", Qt::CaseInsensitive))
        imgfile = "pdf.png";
    else if(mimetype.contains("text/plain", Qt::CaseInsensitive))
        imgfile = "txt.png";
    else if(mimetype.contains(QRegExp("rtf|msword|wordprocessing|opendocument.text", Qt::CaseInsensitive)))
        imgfile = "doc.png";
    else if(mimetype.contains(QRegExp("ms-excel|spreadsheet", Qt::CaseInsensitive)))
        imgfile = "xls.png";
    else if(mimetype.contains(QRegExp("ms-powerpoint|presentation", Qt::CaseInsensitive)))
        imgfile = "ppt.png";
    else if(mimetype.contains("database", Qt::CaseInsensitive))
        imgfile = "odb.png";
    else if (mimetype.contains("html", Qt::CaseInsensitive))
        imgfile = "html.png";
    else if (mimetype.contains("x-uri", Qt::CaseInsensitive))
        imgfile = "url.png";

    /* Images */
    else if (mimetype.contains("image/svg", Qt::CaseInsensitive))
        imgfile = "svg.png";
    else if (mimetype.contains("image/", Qt::CaseInsensitive))
        imgfile = "img.png";

    /* Archives */
    else if (mimetype.contains(QRegExp("ms-cab|stuffit|/zip|x-(compressed|(r|t)ar|7z|(g|b)zip)", Qt::CaseInsensitive)))
        imgfile = "archive.png";

    /* Video */
    else if (mimetype.contains("video/", Qt::CaseInsensitive))
        imgfile = "video.png";

    /* Audio */
    else if (mimetype.contains("audio/", Qt::CaseInsensitive))
        imgfile = "audio.png";

    /* Disc Images */
    else if (mimetype.contains("cd-image", Qt::CaseInsensitive))
        imgfile = "iso.png";

    /* Packages */
    else if (mimetype.contains(QRegExp("x-(apple-diskimage|rpm|deb)", Qt::CaseInsensitive)))
        imgfile = "package.png";

    /* Misc */
    else if (mimetype.contains("bittorrent", Qt::CaseInsensitive))
        imgfile = "torrent.png";

    /* Unknown */
    else
        imgfile = "unknown.png";

    mImageCache->addToCached(imgfile, filenamemd5, theme);
}

bool Support::openFile(QString file)
{
    QString fileLocal = QUrl(file).toLocalFile();
    QFileInfo info(fileLocal);
    if (info.isDir())
       return false;

    return QDesktopServices::openUrl(QUrl(file,QUrl::TolerantMode));
}
