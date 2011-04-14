#ifndef WEBITEM_GLOBAL_H__
#define WEBITEM_GLOBAL_H__

#include <QtCore/qglobal.h>

#if defined(webqgv_EXPORTS)
#  define WEBQGV_EXPORT Q_DECL_EXPORT
#else
#  define WEBQGV_EXPORT Q_DECL_IMPORT
#endif

#endif // WEBITEM_GLOBAL_H__
