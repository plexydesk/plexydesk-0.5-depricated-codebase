#ifndef MIMECONFIG_H__
#define MIMECONFIG_H__

#include <QtCore/qglobal.h>

#if defined(mimetype_EXPORTS)
#  define MIME_EXPORT Q_DECL_EXPORT
#else
#  define MIME_EXPORT Q_DECL_IMPORT
#endif

#endif //MIMECONFIG_H__
