#ifndef SHADERS_GLOBAL_H
#define SHADERS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(plexyshaders_EXPORTS)
#  define SHADERSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHADERSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHADERS_GLOBAL_H
