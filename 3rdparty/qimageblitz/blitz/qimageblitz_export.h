/* 
 Copyright (C) 2007 Christian Ehrlicher <ch.ehrlicher@gmx.de>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef QIMAGEBLITZ_EXPORT_H
#define QIMAGEBLITZ_EXPORT_H

#include <QtCore/QtGlobal>

#ifndef QIMAGEBLITZ_EXPORT
# ifdef Q_OS_WIN
#  ifdef MAKE_QIMAGEBLITZ_LIB
#   define QIMAGEBLITZ_EXPORT Q_DECL_EXPORT
#  else
#   define QIMAGEBLITZ_EXPORT Q_DECL_IMPORT
#  endif
# else
#  define QIMAGEBLITZ_EXPORT Q_DECL_EXPORT
# endif
#endif

#endif // QIMAGEBLITZ_EXPORT_H
