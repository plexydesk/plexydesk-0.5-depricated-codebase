/* 
 Copyright (C) 2004, 2005, 2007 Daniel M. Duley <daniel.duley@verizon.net>

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

/*
 Portions of this software are were originally based on ImageMagick's
 algorithms. ImageMagick is copyrighted under the following conditions:

Copyright (C) 2003 ImageMagick Studio, a non-profit organization dedicated to
making software imaging solutions freely available.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files ("ImageMagick"), to deal
in ImageMagick without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense,  and/or sell
copies of ImageMagick, and to permit persons to whom the ImageMagick is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of ImageMagick.

The software is provided "as is", without warranty of any kind, express or
implied, including but not limited to the warranties of merchantability,
fitness for a particular purpose and noninfringement.  In no event shall
ImageMagick Studio be liable for any claim, damages or other liability,
whether in an action of contract, tort or otherwise, arising from, out of or
in connection with ImageMagick or the use or other dealings in ImageMagick.

Except as contained in this notice, the name of the ImageMagick Studio shall
not be used in advertising or otherwise to promote the sale, use or other
dealings in ImageMagick without prior written authorization from the
ImageMagick Studio.
*/

#include "qimageblitz.h"
#include "private/blitz_p.h"
#include <config-processor.h>
#include "blitzcpu.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
  #include <stdint.h>
#endif

#if defined(__i386__) && ( defined(__GNUC__) || defined(__INTEL_COMPILER) )
#  if defined(HAVE_MMX )
#    define USE_MMX_INLINE_ASM
#  endif
#endif

#define M_EPSILON 1.0e-6
#define M_SQ2PI 2.50662827463100024161235523934010416269302368164062
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RoundToPixel(value) ((unsigned char) ((value) < 0.0 ? 0.0 : \
  ((value) > (float) 255) ? (float) 255 : (value)+0.5))

typedef struct
{
    float red, green, blue, alpha;
} FloatPixel;


#define CONVOLVE_ACC(weight, pixel) \
    r+=((weight))*(qRed((pixel))); g+=((weight))*(qGreen((pixel))); \
    b+=((weight))*(qBlue((pixel)));


QImage Blitz::convolve(QImage &img, int matrix_size, float *matrix)
{
    int i, x, y, w, h, matrix_x, matrix_y;
    int edge = matrix_size/2;
    QRgb *dest, *src, *s, **scanblock;
    float *m, *normalize_matrix, normalize;
    bool overflow = false;

    if(!(matrix_size % 2)){
        qWarning("Blitz::convolve(): kernel width must be an odd number!");
        return(img);
    }

    w = img.width();
    h = img.height();
    if(w < 3 || h < 3){
        qWarning("Blitz::convolve(): Image is too small!");
        return(img);
    }

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 32){ 
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    QImage buffer(w, h, img.format());

    scanblock = new QRgb* [matrix_size];
    normalize_matrix = new float[matrix_size*matrix_size];

    // create normalized matrix
    normalize = 0.0;
    for(i=0; i < matrix_size*matrix_size; ++i)
        normalize += matrix[i];
    if(std::abs(normalize) <=  M_EPSILON)
        normalize = 1.0;
    normalize = 1.0/normalize;
    for(i=0; i < matrix_size*matrix_size; ++i){
        normalize_matrix[i] = normalize*matrix[i];
        if(normalize_matrix[i] < 0.0)
            overflow = true;
    }

    // apply

#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX floating point convolve
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::AMD3DNOW) &&
       BlitzCPUInfo::haveExtension(BlitzCPUInfo::IntegerSSE)){
        //
        //
        // MMX/3dnow version
        //
        //

        __asm__ __volatile__
            ("pxor %%mm7, %%mm7\n\t" : :); // clear for unpacking
        for(y=0; y < h; ++y){
            dest = (QRgb *)buffer.scanLine(y);
            src = (QRgb *)img.scanLine(y);
            // Read in scanlines to pixel neighborhood. If the scanline is outside
            // the image use the top or bottom edge.
            for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                scanblock[i] = (QRgb *)
                    img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
            }
            // Now we are about to start processing scanlines. First handle the
            // part where the pixel neighborhood extends off the left edge.
            for(x=0; x-edge < 0 ; ++x){
                m = normalize_matrix;
                __asm__ __volatile__
                    ("pxor %%mm0, %%mm0\n\t"
                     "pxor %%mm1, %%mm1\n\t"
                     : :);
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y];
                    matrix_x = -edge;
                    while(x+matrix_x < 0){
                        __asm__ __volatile__
                            ("movd (%0), %%mm2\n\t" // mm2: m doubleword
                             "punpckldq %%mm2, %%mm2\n\t"
                             "movd (%1), %%mm3\n\t" // load pixel
                             "punpcklbw %%mm7, %%mm3\n\t"
                             "pshufw $0xE4, %%mm3, %%mm4\n\t"
                             "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                             "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                             "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                             "pi2fd %%mm4, %%mm4\n\t"
                             "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                             "pfmul %%mm2, %%mm4\n\t"

                             "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                             "pfadd %%mm4, %%mm1\n\t"
                             : : "r"(m), "r"(s));
                        ++matrix_x; ++m;
                    }
                    while(matrix_x <= edge){
                        __asm__ __volatile__
                            ("movd (%0), %%mm2\n\t" // mm2: m doubleword
                             "punpckldq %%mm2, %%mm2\n\t"
                             "movd (%1), %%mm3\n\t" // load pixel
                             "punpcklbw %%mm7, %%mm3\n\t"
                             "pshufw $0xE4, %%mm3, %%mm4\n\t"
                             "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                             "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                             "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                             "pi2fd %%mm4, %%mm4\n\t"
                             "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                             "pfmul %%mm2, %%mm4\n\t"

                             "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                             "pfadd %%mm4, %%mm1\n\t"
                             : : "r"(m), "r"(s));
                        ++matrix_x; ++m; ++s;
                    }
                }
                __asm__ __volatile__
                    ("pf2id %%mm0, %%mm0\n\t"
                     "pf2id %%mm1, %%mm1\n\t"

                     "packssdw %%mm0, %%mm1\n\t"
                     "pshufw $0x4E, %%mm1, %%mm1\n\t"
                     "packuswb %%mm7, %%mm1\n\t"
                     "movd %%mm1, (%0)\n\t"
                     : : "r"(dest));
                *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                ++dest;
            }
            // Okay, now process the middle part where the entire neighborhood
            // is on the image.
            for(; x+edge < w; ++x){
                m = normalize_matrix;
                __asm__ __volatile__
                    ("pxor %%mm0, %%mm0\n\t"
                     "pxor %%mm1, %%mm1\n\t"
                     : :);
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y] + (x-edge);
                    for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                        __asm__ __volatile__
                            ("movd (%0), %%mm2\n\t" // mm2: m doubleword
                             "punpckldq %%mm2, %%mm2\n\t"
                             "movd (%1), %%mm3\n\t" // load pixel
                             "punpcklbw %%mm7, %%mm3\n\t"
                             "pshufw $0xE4, %%mm3, %%mm4\n\t"
                             "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                             "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                             "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                             "pi2fd %%mm4, %%mm4\n\t"
                             "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                             "pfmul %%mm2, %%mm4\n\t"

                             "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                             "pfadd %%mm4, %%mm1\n\t"
                             : : "r"(m), "r"(s));
                    }
                }
                __asm__ __volatile__
                    ("pf2id %%mm0, %%mm0\n\t"
                     "pf2id %%mm1, %%mm1\n\t"

                     "packssdw %%mm0, %%mm1\n\t"
                     "pshufw $0x4E, %%mm1, %%mm1\n\t"
                     "packuswb %%mm7, %%mm1\n\t"
                     "movd %%mm1, (%0)\n\t"
                     : : "r"(dest));
                *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                ++dest;
            }
            // Finally process the right part where the neighborhood extends off
            // the right edge of the image
            for(; x < w; ++x){
                m = normalize_matrix;
                __asm__ __volatile__
                    ("pxor %%mm0, %%mm0\n\t"
                     "pxor %%mm1, %%mm1\n\t"
                     : :);
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y];
                    s += x-edge;
                    matrix_x = -edge;
                    while(x+matrix_x < w){
                        __asm__ __volatile__
                            ("movd (%0), %%mm2\n\t" // mm2: m doubleword
                             "punpckldq %%mm2, %%mm2\n\t"
                             "movd (%1), %%mm3\n\t" // load pixel
                             "punpcklbw %%mm7, %%mm3\n\t"
                             "pshufw $0xE4, %%mm3, %%mm4\n\t"
                             "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                             "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                             "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                             "pi2fd %%mm4, %%mm4\n\t"
                             "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                             "pfmul %%mm2, %%mm4\n\t"

                             "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                             "pfadd %%mm4, %%mm1\n\t"
                             : : "r"(m), "r"(s));
                        ++matrix_x, ++m, ++s;
                    }
                    --s;
                    while(matrix_x <= edge){
                        __asm__ __volatile__
                            ("movd (%0), %%mm2\n\t" // mm2: m doubleword
                             "punpckldq %%mm2, %%mm2\n\t"
                             "movd (%1), %%mm3\n\t" // load pixel
                             "punpcklbw %%mm7, %%mm3\n\t"
                             "pshufw $0xE4, %%mm3, %%mm4\n\t"
                             "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                             "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                             "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                             "pi2fd %%mm4, %%mm4\n\t"
                             "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                             "pfmul %%mm2, %%mm4\n\t"

                             "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                             "pfadd %%mm4, %%mm1\n\t"
                             : : "r"(m), "r"(s));
                        ++matrix_x, ++m;
                    }
                }
                __asm__ __volatile__
                    ("pf2id %%mm0, %%mm0\n\t"
                     "pf2id %%mm1, %%mm1\n\t"

                     "packssdw %%mm0, %%mm1\n\t"
                     "pshufw $0x4E, %%mm1, %%mm1\n\t"
                     "packuswb %%mm7, %%mm1\n\t"
                     "movd %%mm1, (%0)\n\t"
                     : : "r"(dest));
                *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                ++dest;
            }
        }
        __asm__ __volatile__ ("emms\n\t" : :);
    }
    else
#endif
    {
        //
        //
        // Non-MMX version
        //
        //

        float r, g, b;
        for(y=0; y < h; ++y){
            src = (QRgb *)img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            // Read in scanlines to pixel neighborhood. If the scanline is outside
            // the image use the top or bottom edge.
            for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                scanblock[i] = (QRgb *)
                    img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
            }
            // Now we are about to start processing scanlines. First handle the
            // part where the pixel neighborhood extends off the left edge.
            for(x=0; x-edge < 0 ; ++x){
                r = g = b = 0.0;
                m = normalize_matrix;
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y];
                    matrix_x = -edge;
                    while(x+matrix_x < 0){
                        CONVOLVE_ACC(*m, *s);
                        ++matrix_x; ++m;
                    }
                    while(matrix_x <= edge){
                        CONVOLVE_ACC(*m, *s);
                        ++matrix_x; ++m; ++s;
                    }
                }
                r = r < 0.0 ? 0.0 : r > 255.0 ? 255.0 : r+0.5;
                g = g < 0.0 ? 0.0 : g > 255.0 ? 255.0 : g+0.5;
                b = b < 0.0 ? 0.0 : b > 255.0 ? 255.0 : b+0.5;
                *dest++ = qRgba((unsigned char)r, (unsigned char)g,
                                (unsigned char)b, qAlpha(*src++));
            }
            // Okay, now process the middle part where the entire neighborhood
            // is on the image.
            for(; x+edge < w; ++x){
                m = normalize_matrix;
                r = g = b = 0.0;
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y] + (x-edge);
                    for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                        CONVOLVE_ACC(*m, *s);
                    }
                }
                r = r < 0.0 ? 0.0 : r > 255.0 ? 255.0 : r+0.5;
                g = g < 0.0 ? 0.0 : g > 255.0 ? 255.0 : g+0.5;
                b = b < 0.0 ? 0.0 : b > 255.0 ? 255.0 : b+0.5;
                *dest++ = qRgba((unsigned char)r, (unsigned char)g,
                                (unsigned char)b, qAlpha(*src++));
            }
            // Finally process the right part where the neighborhood extends off
            // the right edge of the image
            for(; x < w; ++x){
                r = g = b = 0.0;
                m = normalize_matrix;
                for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                    s = scanblock[matrix_y];
                    s += x-edge;
                    matrix_x = -edge;
                    while(x+matrix_x < w){
                        CONVOLVE_ACC(*m, *s);
                        ++matrix_x, ++m, ++s;
                    }
                    --s;
                    while(matrix_x <= edge){
                        CONVOLVE_ACC(*m, *s);
                        ++matrix_x, ++m;
                    }
                }
                r = r < 0.0 ? 0.0 : r > 255.0 ? 255.0 : r+0.5;
                g = g < 0.0 ? 0.0 : g > 255.0 ? 255.0 : g+0.5;
                b = b < 0.0 ? 0.0 : b > 255.0 ? 255.0 : b+0.5;
                *dest++ = qRgba((unsigned char)r, (unsigned char)g,
                                (unsigned char)b, qAlpha(*src++));
            }
        }
    }

    delete[] scanblock;
    delete[] normalize_matrix;
    return(buffer);
}

QImage Blitz::convolveInteger(QImage &img, int matrix_size,
                              int *matrix, int divisor)
{
    int i, x, y, w, h, matrix_x, matrix_y, *m;
    int edge = matrix_size/2;
    QRgb *dest, *src, *s, **scanblock;

    if(!(matrix_size % 2)){
        qWarning("Blitz::convolve(): kernel width must be an odd number!");
        return(img);
    }

    w = img.width();
    h = img.height();
    if(w < 3 || h < 3){
        qWarning("Blitz::convolve(): Image is too small!");
        return(img);
    }

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 32){ 
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }

    QImage buffer(w, h, img.format());
    scanblock = new QRgb* [matrix_size];

    if(!divisor){
#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX integer convolve
#endif
        if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::MMX)){
            //
            // No divisor MMX version
            //
            __asm__ __volatile__
                ("pxor %%mm7, %%mm7\n\t" : :); // clear for unpacking
            for(y=0; y < h; ++y){
                src = (QRgb *)img.scanLine(y);
                dest = (QRgb *)buffer.scanLine(y);
                // Read in scanlines to pixel neighborhood. If the scanline is outside
                // the image use the top or bottom edge.
                for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                    scanblock[i] = (QRgb *)
                        img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
                }
                // Now we are about to start processing scanlines. First handle the
                // part where the pixel neighborhood extends off the left edge.
                for(x=0; x-edge < 0 ; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc

                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        matrix_x = -edge;
                        while(x+matrix_x < 0){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x; ++m;
                        }
                        while(matrix_x <= edge){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x; ++m; ++s;
                        }
                    }
                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
                // Okay, now process the middle part where the entire neighborhood
                // is on the image.
                for(; x+edge < w; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y] + (x-edge);
                        for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                        }
                    }

                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
                // Finally process the right part where the neighborhood extends off
                // the right edge of the image
                for(; x < w; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        s += x-edge;
                        matrix_x = -edge;
                        while(x+matrix_x < w){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x, ++m, ++s;
                        }
                        --s;
                        while(matrix_x <= edge){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x, ++m;
                        }
                    }
                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
            }
            __asm__ __volatile__ ("emms\n\t" : :);
        }
        else
#endif
        {
            //
            // No divisor non-MMX version
            //
            int r, g, b;
            for(y=0; y < h; ++y){
                src = (QRgb *)img.scanLine(y);
                dest = (QRgb *)buffer.scanLine(y);
                // Read in scanlines to pixel neighborhood. If the scanline is outside
                // the image use the top or bottom edge.
                for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                    scanblock[i] = (QRgb *)
                        img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
                }
                // Now we are about to start processing scanlines. First handle the
                // part where the pixel neighborhood extends off the left edge.
                for(x=0; x-edge < 0 ; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        matrix_x = -edge;
                        while(x+matrix_x < 0){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x; ++m;
                        }
                        while(matrix_x <= edge){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x; ++m; ++s;
                        }
                    }
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
                // Okay, now process the middle part where the entire neighborhood
                // is on the image.
                for(; x+edge < w; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y] + (x-edge);
                        for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                            CONVOLVE_ACC(*m, *s);
                        }
                    }
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
                // Finally process the right part where the neighborhood extends off
                // the right edge of the image
                for(; x < w; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        s += x-edge;
                        matrix_x = -edge;
                        while(x+matrix_x < w){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x, ++m, ++s;
                        }
                        --s;
                        while(matrix_x <= edge){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x, ++m;
                        }
                    }
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
            }
        }
    }
    else{
#ifdef USE_MMX_INLINE_ASM
        // FIXME: TODO
        if(/*BlitzCPUInfo::haveExtension(BlitzCPUInfo::AMD3DNOW)*/false){
            //
            // Divisor 3dnow version
            //
            __asm__ __volatile__
                ("pxor %%mm7, %%mm7\n\t" : :); // clear for unpacking

            for(y=0; y < h; ++y){
                src = (QRgb *)img.scanLine(y);
                dest = (QRgb *)buffer.scanLine(y);
                // Read in scanlines to pixel neighborhood. If the scanline is outside
                // the image use the top or bottom edge.
                for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                    scanblock[i] = (QRgb *)
                        img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
                }
                // Now we are about to start processing scanlines. First handle the
                // part where the pixel neighborhood extends off the left edge.
                for(x=0; x-edge < 0 ; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc

                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        matrix_x = -edge;
                        while(x+matrix_x < 0){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x; ++m;
                        }
                        while(matrix_x <= edge){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x; ++m; ++s;
                        }
                    }
                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
                // Okay, now process the middle part where the entire neighborhood
                // is on the image.
                for(; x+edge < w; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y] + (x-edge);
                        for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                        }
                    }

                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
                // Finally process the right part where the neighborhood extends off
                // the right edge of the image
                for(; x < w; ++x){
                    m = matrix;
                    __asm__ __volatile__
                        ("pxor %%mm0, %%mm0\n\t" : :); // clear acc
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        s += x-edge;
                        matrix_x = -edge;
                        while(x+matrix_x < w){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x, ++m, ++s;
                        }
                        --s;
                        while(matrix_x <= edge){
                            __asm__ __volatile__
                                ("movd (%0), %%mm1\n\t" // mm1: matrix
                                 "punpckldq %%mm1, %%mm1\n\t"
                                 "packssdw %%mm1, %%mm1\n\t"
                                 "movd (%1), %%mm2\n\t" // mm2: pixel
                                 "punpcklbw %%mm7, %%mm2\n\t"
                                 "pmullw %%mm1, %%mm2\n\t" // multiply
                                 "paddsw %%mm2, %%mm0\n\t" // add to acc
                                 : : "r"(m), "r"(s));
                            ++matrix_x, ++m;
                        }
                    }
                    __asm__ __volatile__
                        ("packuswb %%mm0, %%mm0\n\t" // pack and write
                         "movd %%mm0, (%0)\n\t"
                         : : "r"(dest));
                    *dest = BlitzPrivate::setAlpha(*dest, qAlpha(*src++));
                    ++dest;
                }
            }
            __asm__ __volatile__ ("emms\n\t" : :);
        }
        else
#endif
        {
            //
            // Divisor, no 3dnow
            //
            int r, g, b;
            for(y=0; y < h; ++y){
                src = (QRgb *)img.scanLine(y);
                dest = (QRgb *)buffer.scanLine(y);
                // Read in scanlines to pixel neighborhood. If the scanline is outside
                // the image use the top or bottom edge.
                for(x=y-edge, i=0; x <= y+edge; ++i, ++x){
                    scanblock[i] = (QRgb *)
                        img.scanLine((x < 0) ? 0 : (x > h-1) ? h-1 : x);
                }
                // Now we are about to start processing scanlines. First handle the
                // part where the pixel neighborhood extends off the left edge.
                for(x=0; x-edge < 0 ; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        matrix_x = -edge;
                        while(x+matrix_x < 0){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x; ++m;
                        }
                        while(matrix_x <= edge){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x; ++m; ++s;
                        }
                    }
                    r /= divisor; g /= divisor; b /= divisor;
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
                // Okay, now process the middle part where the entire neighborhood
                // is on the image.
                for(; x+edge < w; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y] + (x-edge);
                        for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++m, ++s){
                            CONVOLVE_ACC(*m, *s);
                        }
                    }
                    r /= divisor; g /= divisor; b /= divisor;
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
                // Finally process the right part where the neighborhood extends off
                // the right edge of the image
                for(; x < w; ++x){
                    r = g = b = 0;
                    m = matrix;
                    for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                        s = scanblock[matrix_y];
                        s += x-edge;
                        matrix_x = -edge;
                        while(x+matrix_x < w){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x, ++m, ++s;
                        }
                        --s;
                        while(matrix_x <= edge){
                            CONVOLVE_ACC(*m, *s);
                            ++matrix_x, ++m;
                        }
                    }
                    r /= divisor; g /= divisor; b /= divisor;
                    *dest++ = qRgba((unsigned char)qBound(0, r, 255),
                                    (unsigned char)qBound(0, g, 255),
                                    (unsigned char)qBound(0, b, 255),
                                    qAlpha(*src++));
                }
            }
        }
    }

    delete[] scanblock;
    return(buffer);
}

int BlitzPrivate::defaultConvolveMatrixSize(float radius, float sigma,
                                            bool quality)
{
    int i, matrix_size;
    float normalize, value;
    float sigma2 = sigma*sigma*2.0;
    float sigmaSQ2PI = M_SQ2PI*sigma;
    int max = quality ? 65535 : 255;

    if(sigma == 0.0){
        qWarning("Blitz::convolve(): Zero sigma is invalid!");
        return(5);
    }

    if(radius > 0.0)
        return((int)(2.0*std::ceil(radius)+1.0));

    matrix_size = 5;
    do{
        normalize = 0.0;
        for(i=(-matrix_size/2); i <= (matrix_size/2); ++i)
            normalize += std::exp(-((float) i*i)/sigma2) / sigmaSQ2PI;
        i = matrix_size/2;
        value = std::exp(-((float) i*i)/sigma2) / sigmaSQ2PI / normalize;
        matrix_size += 2;
    } while((int)(max*value) > 0);

    matrix_size-=4;
    return(matrix_size);
}

float* BlitzPrivate::getBlurKernel(int &kernel_width, float sigma)
{
#define KernelRank 3

    float alpha, normalize, *kernel;
    int bias;
    long i;

    if(sigma == 0.0)
        return(false);
    if(kernel_width == 0)
        kernel_width = 3;

    kernel = new float[kernel_width+1];
    memset(kernel, 0, (kernel_width+1)*sizeof(float));
    bias = KernelRank*kernel_width/2;
    for(i=(-bias); i <= bias; ++i){
        alpha = std::exp(-((float) i*i)/(2.0*KernelRank*KernelRank*sigma*sigma));
        kernel[(i+bias)/KernelRank] += alpha/(M_SQ2PI*sigma);
    }

    normalize = 0;
    for(i=0; i < kernel_width; ++i)
        normalize += kernel[i];
    for(i=0; i < kernel_width; ++i)
        kernel[i] /= normalize;
    return(kernel);
}

void BlitzPrivate::blurScanLine(float *kernel, int kern_width,
                                QRgb *source, QRgb *destination,
                                int columns, int offset)
{
    FloatPixel aggregate, zero;
    float scale, *k;
    QRgb *src, *dest;
    int i, x;

#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX gaussian blur
#endif
    //
    //
    // MMX Version
    //
    //

    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::AMD3DNOW) &&
       BlitzCPUInfo::haveExtension(BlitzCPUInfo::IntegerSSE)){
        if(kern_width > columns){
            memset(&zero, 0, sizeof(FloatPixel));
            for(dest=destination, x=0; x < columns; ++x, dest+=offset){
                aggregate = zero;
                scale = 0.0;
                k = kernel;
                src = source;
                for(i=0; i < columns; ++k, src+=offset){
                    if((i >= (x-kern_width/2)) && (i <= (x+kern_width/2))){
                        aggregate.red += (*k)*qRed(*src);
                        aggregate.green += (*k)*qGreen(*src);
                        aggregate.blue += (*k)*qBlue(*src);
                        aggregate.alpha += (*k)*qAlpha(*src);
                    }

                    if(((i+kern_width/2-x) >= 0) && ((i+kern_width/2-x) < kern_width))
                        scale += kernel[i+kern_width/2-x];
                }
                scale = 1.0/scale;
                *dest = qRgba((unsigned char)(scale*(aggregate.red+0.5)),
                              (unsigned char)(scale*(aggregate.green+0.5)),
                              (unsigned char)(scale*(aggregate.blue+0.5)),
                              (unsigned char)(scale*(aggregate.alpha+0.5)));
            }
            return;
        }

        // blur
        __asm__ __volatile__ ("pxor %%mm7, %%mm7\n\t" : : ); // for unpacking
        for(dest=destination, x=0; x < kern_width/2; ++x, dest+=offset){
            k = kernel+kern_width/2-x;
            src = source;
            __asm__ __volatile__ ("pxor %%mm0, %%mm0\n\t" // mm0: BG acc
                                  "pxor %%mm1, %%mm1\n\t" // mm1: RA acc
                                  "pxor %%mm6, %%mm6\n\t" // mm6: scale
                                  : : );
            for(i=kern_width/2-x; i < kern_width; ++i, ++k, src+=offset){
                __asm__ __volatile__
                    ("movd (%0), %%mm2\n\t" // mm2: k doubleword
                     "punpckldq %%mm2, %%mm2\n\t"
                     "pfadd %%mm2, %%mm6\n\t" // add to scale

                     "movd (%1), %%mm3\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm3\n\t"
                     "pshufw $0xE4, %%mm3, %%mm4\n\t"
                     "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                     "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                     "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                     "pi2fd %%mm4, %%mm4\n\t"
                     "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                     "pfmul %%mm2, %%mm4\n\t"

                     "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                     "pfadd %%mm4, %%mm1\n\t"
                     : : "r"(k), "r"(src));
            }
            __asm__ __volatile__
                ("pfrcp %%mm6, %%mm5\n\t" // reciprocal
                 "pfrcpit1 %%mm5, %%mm6\n\t" // expand to 24bit
                 "pfrcpit2 %%mm5, %%mm6\n\t"

                 "pfmul %%mm6, %%mm0\n\t" // multiply
                 "pfmul %%mm6, %%mm1\n\t"

                 "pf2id %%mm0, %%mm0\n\t" // and write
                 "pf2id %%mm1, %%mm1\n\t"
                 "packssdw %%mm0, %%mm1\n\t"
                 "pshufw $0x4E, %%mm1, %%mm1\n\t"
                 "packuswb %%mm7, %%mm1\n\t"
                 "movd %%mm1, (%0)\n\t"
                 : : "r"(dest));
        }
        for(; x < (columns-kern_width/2); ++x, dest+=offset){
            __asm__ __volatile__ ("pxor %%mm0, %%mm0\n\t" // mm0: BG acc
                                  "pxor %%mm1, %%mm1\n\t" // mm1: RA acc
                                  : : );
            k = kernel;
            src = source+((x-kern_width/2)*offset);
            for(i=0; i < kern_width; ++i, ++k, src+=offset){
                __asm__ __volatile__
                    ("movd (%0), %%mm2\n\t" // mm2: k doubleword
                     "punpckldq %%mm2, %%mm2\n\t"

                     "movd (%1), %%mm3\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm3\n\t"
                     "pshufw $0xE4, %%mm3, %%mm4\n\t"
                     "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                     "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                     "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                     "pi2fd %%mm4, %%mm4\n\t"
                     "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                     "pfmul %%mm2, %%mm4\n\t"

                     "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                     "pfadd %%mm4, %%mm1\n\t"
                     : : "r"(k), "r"(src));
            }
            __asm__ __volatile__
                ("pf2id %%mm0, %%mm0\n\t" // write
                 "pf2id %%mm1, %%mm1\n\t"
                 "packssdw %%mm0, %%mm1\n\t"
                 "pshufw $0x4E, %%mm1, %%mm1\n\t"
                 "packuswb %%mm7, %%mm1\n\t"
                 "movd %%mm1, (%0)\n\t"
                 : : "r"(dest));
        }
        for(; x < columns; ++x, dest+=offset){
            k = kernel;
            src = source+((x-kern_width/2)*offset);
            __asm__ __volatile__ ("pxor %%mm0, %%mm0\n\t" // mm0: BG acc
                                  "pxor %%mm1, %%mm1\n\t" // mm1: RA acc
                                  "pxor %%mm6, %%mm6\n\t" // mm6: scale
                                  : : );
            for(i=0; i < (columns-x+kern_width/2); ++i, ++k, src+=offset){
                __asm__ __volatile__
                    ("movd (%0), %%mm2\n\t" // mm2: k doubleword
                     "punpckldq %%mm2, %%mm2\n\t"
                     "pfadd %%mm2, %%mm6\n\t" // add to scale

                     "movd (%1), %%mm3\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm3\n\t"
                     "pshufw $0xE4, %%mm3, %%mm4\n\t"
                     "punpcklwd %%mm7, %%mm3\n\t" // mm3: BG
                     "punpckhwd %%mm7, %%mm4\n\t" // mm4: RA

                     "pi2fd %%mm3, %%mm3\n\t" // convert to fp...
                     "pi2fd %%mm4, %%mm4\n\t"
                     "pfmul %%mm2, %%mm3\n\t" // ...and multiply
                     "pfmul %%mm2, %%mm4\n\t"

                     "pfadd %%mm3, %%mm0\n\t" // add to accumulator
                     "pfadd %%mm4, %%mm1\n\t"
                     : : "r"(k), "r"(src));
            }
            __asm__ __volatile__
                ("pfrcp %%mm6, %%mm5\n\t" // reciprocal
                 "pfrcpit1 %%mm5, %%mm6\n\t" // expand to 24bit
                 "pfrcpit2 %%mm5, %%mm6\n\t"

                 "pfmul %%mm6, %%mm0\n\t" // multiply
                 "pfmul %%mm6, %%mm1\n\t"

                 "pf2id %%mm0, %%mm0\n\t" // and write
                 "pf2id %%mm1, %%mm1\n\t"
                 "packssdw %%mm0, %%mm1\n\t"
                 "pshufw $0x4E, %%mm1, %%mm1\n\t"
                 "packuswb %%mm7, %%mm1\n\t"
                 "movd %%mm1, (%0)\n\t"
                 : : "r"(dest));
        }

        __asm__ __volatile__ ("emms\n\t" : :);
    }
    else
#endif
    {
        //
        //
        // Non-MMX version
        //
        //

        memset(&zero, 0, sizeof(FloatPixel));
        if(kern_width > columns){
            for(dest=destination, x=0; x < columns; ++x, dest+=offset){
                aggregate = zero;
                scale = 0.0;
                k = kernel;
                src = source;
                for(i=0; i < columns; ++k, src+=offset){
                    if((i >= (x-kern_width/2)) && (i <= (x+kern_width/2))){
                        aggregate.red += (*k)*qRed(*src);
                        aggregate.green += (*k)*qGreen(*src);
                        aggregate.blue += (*k)*qBlue(*src);
                        aggregate.alpha += (*k)*qAlpha(*src);
                    }

                    if(((i+kern_width/2-x) >= 0) && ((i+kern_width/2-x) < kern_width))
                        scale += kernel[i+kern_width/2-x];
                }
                scale = 1.0/scale;
                *dest = qRgba((unsigned char)(scale*(aggregate.red+0.5)),
                              (unsigned char)(scale*(aggregate.green+0.5)),
                              (unsigned char)(scale*(aggregate.blue+0.5)),
                              (unsigned char)(scale*(aggregate.alpha+0.5)));
            }
            return;
        }

        // blur
        for(dest=destination, x=0; x < kern_width/2; ++x, dest+=offset){
            aggregate = zero; // put this stuff in loop initializer once tested
            scale = 0.0;
            k = kernel+kern_width/2-x;
            src = source;
            for(i=kern_width/2-x; i < kern_width; ++i, ++k, src+=offset){
                aggregate.red += (*k)*qRed(*src);
                aggregate.green += (*k)*qGreen(*src);
                aggregate.blue += (*k)*qBlue(*src);
                aggregate.alpha += (*k)*qAlpha(*src);
                scale += (*k);
            }
            scale = 1.0/scale;
            *dest = qRgba((unsigned char)(scale*(aggregate.red+0.5)),
                          (unsigned char)(scale*(aggregate.green+0.5)),
                          (unsigned char)(scale*(aggregate.blue+0.5)),
                          (unsigned char)(scale*(aggregate.alpha+0.5)));
        }
        for(; x < (columns-kern_width/2); ++x, dest+=offset){
            aggregate = zero;
            k = kernel;
            src = source+((x-kern_width/2)*offset);
            for(i=0; i < kern_width; ++i, ++k, src+=offset){
                aggregate.red += (*k)*qRed(*src);
                aggregate.green += (*k)*qGreen(*src);
                aggregate.blue += (*k)*qBlue(*src);
                aggregate.alpha += (*k)*qAlpha(*src);
            }
            *dest = qRgba((unsigned char)(aggregate.red+0.5),
                          (unsigned char)(aggregate.green+0.5),
                          (unsigned char)(aggregate.blue+0.5),
                          (unsigned char)(aggregate.alpha+0.5));
        }
        for(; x < columns; ++x, dest+=offset){
            aggregate = zero;
            scale = 0;
            k = kernel;
            src = source+((x-kern_width/2)*offset);
            for(i=0; i < (columns-x+kern_width/2); ++i, ++k, src+=offset){
                aggregate.red += (*k)*qRed(*src);
                aggregate.green += (*k)*qGreen(*src);
                aggregate.blue += (*k)*qBlue(*src);
                aggregate.alpha += (*k)*qAlpha(*src);
                scale += (*k);
            }
            scale = 1.0/scale;
            *dest = qRgba((unsigned char)(scale*(aggregate.red+0.5)),
                          (unsigned char)(scale*(aggregate.green+0.5)),
                          (unsigned char)(scale*(aggregate.blue+0.5)),
                          (unsigned char)(scale*(aggregate.alpha+0.5)));
        }
    }
}

QImage Blitz::gaussianBlur(QImage &img, float radius, float sigma)
{
    int kern_width, x, y, w, h;
    QRgb *src;
    float *k = NULL;

    if(sigma == 0.0){
        qWarning("Blitz::gaussianBlur(): Zero sigma is not valid!");
        return(img);
    }

    // figure out optimal kernel width
    if(radius > 0){
        kern_width = (int)(2*std::ceil(radius)+1);
        k = BlitzPrivate::getBlurKernel(kern_width, sigma);
    }
    else{
        float *last_kernel = NULL;
        kern_width = 3;
        k = BlitzPrivate::getBlurKernel(kern_width, sigma);
        while((long)(255*k[0]) > 0){
            if(last_kernel != NULL)
                delete[] last_kernel;
            last_kernel = k;
            kern_width += 2;
            k = BlitzPrivate::getBlurKernel(kern_width, sigma);
        }
        if(last_kernel != NULL){
            delete[] k;
            kern_width -= 2;
            k = last_kernel;
        }
    }

    if(kern_width < 3){
        qWarning("Blitz::gaussianBlur(): radius is too small!");
        return(img);
    }

    // allocate destination image
    w = img.width();
    h = img.height();
    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    QImage buffer(w, h, img.format());

    //blur image rows
    for(y=0; y < h; ++y)
        BlitzPrivate::blurScanLine(k, kern_width, (QRgb *)img.scanLine(y),
                                   (QRgb *)buffer.scanLine(y), img.width(), 1);

    // blur image columns
    src = (QRgb *)buffer.scanLine(0);
    for(x=0; x < w; ++x)
        BlitzPrivate::blurScanLine(k, kern_width, src+x, src+x, img.height(),
                                   img.width());
    // finish up
    delete[] k;
    return(buffer);
}

QImage Blitz::gaussianSharpen(QImage &img, float radius, float sigma,
                              EffectQuality quality)
{
    if(sigma == 0.0){
        qWarning("Blitz::gaussianSharpen(): Zero sigma is invalid!");
        return(img);
    }

    int matrix_size = BlitzPrivate::defaultConvolveMatrixSize(radius, sigma,
                                                              quality == High);
    int len = matrix_size*matrix_size;
    float alpha, *matrix = new float[len];
    float sigma2 = sigma*sigma*2.0;
    float sigmaPI2 = 2.0*M_PI*sigma*sigma;

    int half = matrix_size/2;
    int x, y, i=0, j=half;
    float normalize=0.0;
    for(y=(-half); y <= half; ++y, --j){
        for(x=(-half); x <= half; ++x, ++i){
            alpha = std::exp(-((float)x*x+y*y)/sigma2);
            matrix[i] = alpha/sigmaPI2;
            normalize += matrix[i];
        }
    }

    matrix[i/2]=(-2.0)*normalize;
    QImage result(convolve(img, matrix_size, matrix));
    delete[] matrix;
    return(result);
}

QImage Blitz::antialias(QImage &img)
{
    // nice little example method
    int matrix[] = {
        1, 2, 1,
        2, 8, 2,
        1, 2, 1
    };
    return(convolveInteger(img, 3, matrix, 20));
}

QImage Blitz::emboss(QImage &img, float radius, float sigma,
                     EffectQuality quality)
{
    if(sigma == 0.0){
        qWarning("Blitz::emboss(): Zero sigma is invalid!");
        return(img);
    }

    int matrix_size = BlitzPrivate::defaultConvolveMatrixSize(radius, sigma,
                                                              quality == High);
    int len = matrix_size*matrix_size;

    float alpha, *matrix = new float[len];
    float sigma2 = sigma*sigma*2.0;
    float sigmaPI2 = 2.0*M_PI*sigma*sigma;

    int half = matrix_size/2;
    int x, y, i=0, j=half;
    for(y=(-half); y <= half; ++y, --j){
        for(x=(-half); x <= half; ++x, ++i){
            alpha = std::exp(-((float)x*x+y*y)/sigma2);
            matrix[i]=((x < 0) || (y < 0) ? -8.0 : 8.0)*alpha/sigmaPI2;
            if(x == j)
                matrix[i]=0.0;
        }
    }
    QImage result(convolve(img, matrix_size, matrix));
    delete[] matrix;
    equalize(result);
    return(result);
}

QImage Blitz::convolveEdge(QImage &img, float radius,
                           EffectQuality quality)
{
    int i, matrix_size = BlitzPrivate::defaultConvolveMatrixSize(radius, 0.5,
                                                                 quality == High);
    int len = matrix_size*matrix_size;
    int *matrix = new int[len];
    for(i=0; i < len; ++i)
        matrix[i] = -1;
    matrix[i/2] = len-1;

    QImage result(convolveInteger(img, matrix_size, matrix, 0));
    delete[] matrix;
    return(result);
}


