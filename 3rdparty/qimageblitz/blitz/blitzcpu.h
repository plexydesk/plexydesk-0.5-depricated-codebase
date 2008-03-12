#ifndef __BLITZ_CPU_H
#define __BLITZ_CPU_H

/**
 * CPUID code: This tests the CPU id flags for MMX, SSE, 3dnow, etc...
 * support. This is based on KCPUInfo by Fredrik Hoglund.
 *
 * Copyright (C) 2003 Fredrik Hoglund <fredrik@kde.org>
 * Copyright (C) 2004 Daniel M. Duley <daniel.duley@verizon.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "qimageblitz_export.h"
#include <QtCore/QtGlobal>

/**
 * MMX packed value data types. If you don't write MMX assembler code you
 * can ignore these.
 *
 * @author Daniel M. Duley <daniel.duley@verizon.net>
 * @short MMX data types
 */
namespace MMX{
    typedef struct { quint32 d[2]; } Pack2;
    typedef struct { quint16 d[4]; } Pack4;
    typedef struct { quint8  d[8]; } Pack8;
}

/**
 * This class determines what extensions the computer's CPU supports. These
 * include MMX, SSE, 3dnow, and AltiVec. If you don't write assembler code
 * you probably can ignore this class.
 *
 * @short CPU features identification
 * @author Fredrik Hoglund <fredrik@kde.org>
 * @author Daniel M. Duley <daniel.duley@verizon.net>
 */
class BlitzCPUInfo{
public:
    enum Extension {
        MMX = 1 << 0,
        IntegerSSE = 1 << 1,
        SSE = 1 << 2,
        SSE2 = 1 << 3,
        AMD3DNOW = 1 << 4,
        AMD3DNOW2 = 1 << 5,
        AltiVec = 1 << 6
    };
    /**
     * Returns true if the CPU supports the given extension.
     */
    static bool haveExtension(unsigned int extension);
};


#endif

