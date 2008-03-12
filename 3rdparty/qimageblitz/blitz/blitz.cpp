/* 
 Copyright (C) 1998, 1999, 2001, 2002, 2004, 2005, 2007
      Daniel M. Duley <daniel.duley@verizon.net>
 (C) 2004 Zack Rusin <zack@kde.org>
 (C) 2000 Josef Weidendorfer <weidendo@in.tum.de>
 (C) 1999 Geert Jansen <g.t.jansen@stud.tue.nl>
 (C) 1998, 1999 Christian Tibirna <ctibirna@total.net>
 (C) 1998, 1999 Dirk Mueller <mueller@kde.org>

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

#define _USE_MATH_DEFINES   // for msvc

#include "qimageblitz.h"
#include "private/blitz_p.h"
#include <config-processor.h>
#include "blitzcpu.h"
#include "private/interpolate.h"
#include "private/inlinehsv.h"
#include <cmath>
#ifndef M_PI
// Some cmath doesn't define it, try from the older source
#include "math.h"
#endif
#include <string.h>
#include <QVector>
#include <QColor>

#if defined(__i386__) && ( defined(__GNUC__) || defined(__INTEL_COMPILER) )
#  if defined(HAVE_MMX )
#    define USE_MMX_INLINE_ASM
#  endif
#endif

QImage& Blitz::despeckle(QImage &img)
{
    if(img.isNull())
        return(img);

    int length, x, y, j, i;
    QRgb *src, *dest;
    unsigned char *buffer, *pixels;
    int w = img.width();
    int h = img.height();

    static const int
        X[4]= {0, 1, 1,-1},
        Y[4]= {1, 0, 1, 1};

    length = (img.width()+2)*(img.height()+2);
    pixels = new unsigned char[length];
    buffer = new unsigned char[length];

    if(img.depth() != 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    else if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);

    // Do each channel. This originally was in one loop with an "if" statement
    // for each channel, but I unrolled it because a lot goes on and it needs
    // any speed help it can get >:/ (mosfet)

    // Red
    (void)memset(pixels, 0, length);
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        src = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++src, ++j)
            pixels[j] = qRed(*src);
    }
    (void)memset(buffer, 0, length);
    for(i=0; i < 4; ++i){
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, -1);
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, -1);
    }
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        dest = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++dest, ++j)
            *dest = qRgba(pixels[j], qGreen(*dest), qBlue(*dest),
                          qAlpha(*dest));
    }

    // Green
    (void)memset(pixels, 0, length);
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        src = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++src, ++j)
            pixels[j] = qGreen(*src);
    }
    (void)memset(buffer, 0, length);
    for(i=0; i < 4; ++i){
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, -1);
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, -1);
    }
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        dest = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++dest, ++j)
            *dest = qRgba(qRed(*dest), pixels[j], qBlue(*dest),
                          qAlpha(*dest));
    }

    // Blue
    (void)memset(pixels, 0, length);
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        src = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++src, ++j)
            pixels[j] = qBlue(*src);
    }
    (void)memset(buffer, 0, length);
    for(i=0; i < 4; ++i){
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, 1);
        BlitzPrivate::hull(-X[i], -Y[i], w, h, pixels, buffer, -1);
        BlitzPrivate::hull(X[i], Y[i], w, h, pixels, buffer, -1);
    }
    j = w+2;
    for(y=0; y < h; ++y, ++j){
        dest = (QRgb *)img.scanLine(y);
        ++j;
        for(x=w-1; x >= 0; --x, ++dest, ++j)
            *dest = qRgba(qRed(*dest), qGreen(*dest), pixels[j],
                          qAlpha(*dest));
    }

    delete[] pixels;
    delete[] buffer;
    return(img);
}

QImage Blitz::blur(QImage &img, int radius)
{
    QRgb *p1, *p2;
    int x, y, w, h, mx, my, mw, mh, mt, xx, yy;
    int a, r, g, b;
    int *as, *rs, *gs, *bs;

    if(radius < 1 || img.isNull() || img.width() < (radius << 1))
        return(img);

    w = img.width();
    h = img.height();

    if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);
    QImage buffer(w, h, img.hasAlphaChannel() ?
                  QImage::Format_ARGB32 : QImage::Format_RGB32);

    as = new int[w];
    rs = new int[w];
    gs = new int[w];
    bs = new int[w];

    QVector<QRgb> colorTable;
    if(img.format() == QImage::Format_Indexed8)
        colorTable = img.colorTable();

    for(y = 0; y < h; y++){
        my = y - radius;
        mh = (radius << 1) + 1;
        if(my < 0){
            mh += my;
            my = 0;
        }
        if((my + mh) > h)
            mh = h - my;

        p1 = (QRgb *)buffer.scanLine(y);
        memset(as, 0, w * sizeof(int));
        memset(rs, 0, w * sizeof(int));
        memset(gs, 0, w * sizeof(int));
        memset(bs, 0, w * sizeof(int));

        if(img.format() == QImage::Format_ARGB32_Premultiplied){
            QRgb pixel;
            for(yy = 0; yy < mh; yy++){
                p2 = (QRgb *)img.scanLine(yy + my);
                for(x = 0; x < w; x++, p2++){
                    pixel = BlitzPrivate::convertFromPremult(*p2);
                    as[x] += qAlpha(pixel);
                    rs[x] += qRed(pixel);
                    gs[x] += qGreen(pixel);
                    bs[x] += qBlue(pixel);
                }
            }
        }
        else if(img.format() == QImage::Format_Indexed8){
            QRgb pixel;
            unsigned char *ptr;
            for(yy = 0; yy < mh; yy++){
                ptr = (unsigned char *)img.scanLine(yy + my);
                for(x = 0; x < w; x++, ptr++){
                    pixel = colorTable[*ptr];
                    as[x] += qAlpha(pixel);
                    rs[x] += qRed(pixel);
                    gs[x] += qGreen(pixel);
                    bs[x] += qBlue(pixel);
                }
            }
        }
        else{
            for(yy = 0; yy < mh; yy++){
                p2 = (QRgb *)img.scanLine(yy + my);
                for(x = 0; x < w; x++, p2++){
                    as[x] += qAlpha(*p2);
                    rs[x] += qRed(*p2);
                    gs[x] += qGreen(*p2);
                    bs[x] += qBlue(*p2);
                }
            }
        }

        for(x = 0; x < w; x++){
            a = r = g = b = 0;
            mx = x - radius;
            mw = (radius << 1) + 1;
            if(mx < 0){
                mw += mx;
                mx = 0;
                }
            if((mx + mw) > w)
                mw = w - mx;
            mt = mw * mh;
            for(xx = mx; xx < (mw + mx); xx++){
                a += as[xx];
                r += rs[xx];
                g += gs[xx];
                b += bs[xx];
            }
            a = a / mt;
            r = r / mt;
            g = g / mt;
            b = b / mt;
            *p1++ = qRgba(r, g, b, a);
        }
    }
    delete[] as;
    delete[] rs;
    delete[] gs;
    delete[] bs;

    return(buffer);
}

QImage Blitz::sharpen(QImage &img, int radius)
{
    if(img.isNull() || radius < 1)
        return(img);
    if(img.depth() != 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    else if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);

    QImage buffer(img.width(), img.height(), img.format());
    int a, r, g, b, x, y;
    int w = img.width();
    int h = img.height();
    QRgb *src, *dest;

    memcpy(buffer.scanLine(0), img.scanLine(0), img.bytesPerLine());
    for(y=1; y < h-1; ++y){
        src = (QRgb *)img.scanLine(y);
        dest = (QRgb *)buffer.scanLine(y);
        *dest++ = *src++;
        for(x=1; x < w-1; ++x){
            r = qRed(*src)*5;
            g = qGreen(*src)*5;
            b = qBlue(*src)*5;
            a = qAlpha(*src)*5;

            r -= qRed(*(src-1));
            g -= qGreen(*(src-1));
            b -= qBlue(*(src-1));
            a -= qAlpha(*(src-1));
            r -= qRed(*(src+1));
            g -= qGreen(*(src+1));
            b -= qBlue(*(src+1));
            a -= qAlpha(*(src+1));

            r -= qRed(*(src-w));
            g -= qGreen(*(src-w));
            b -= qBlue(*(src-w));
            a -= qAlpha(*(src-w));
            r -= qRed(*(src+w));
            g -= qGreen(*(src+w));
            b -= qBlue(*(src+w));
            a -= qAlpha(*(src+w));

            r = (r & ((~r) >> 16));
            r = ((r | ((r & 256) - ((r & 256) >> 8))));
            g = (g & ((~g) >> 16));
            g = ((g | ((g & 256) - ((g & 256) >> 8))));
            b = (b & ((~b) >> 16));
            b = ((b | ((b & 256) - ((b & 256) >> 8))));
            a = (a & ((~a) >> 16));
            a = ((a | ((a & 256) - ((a & 256) >> 8))));

            *dest = qRgba(r, g, b, a);
            ++src; ++dest;
        }
        *dest++ = *src++;
    }
    memcpy(buffer.scanLine(h-1), img.scanLine(h-1), img.bytesPerLine());
    return(buffer);
}

/*
 * I was looking for a quick way to do edge detection and found
 * the Sobel method. This is a gradient method that applies two 3x3
 * matrixes. These matrixes are:
 *
 * x:  -1, 0, 1      y:  1,  2,  1
 *     -2, 0, 2          0,  0,  0
 *     -1, 0, 1         -1, -2, -1
 *
 * After the matrix is applied you simply calculate the magnitude by
 * |x| + |y|.
 *
 * The one problem w/ this is most descriptions of the algorithm I've
 * seen assume grayscale data so your working with the intensity of the
 * pixel. We do each color channel separately. This is probably wrong,
 * but all the implementations I've seen do this...
 * (mosfet)
 */

// Accumulates the results of applying x and y Sobel masks
#define SOBEL(xm, ym, pixel) \
    xR += qRed((pixel))*(xm); xG += qGreen((pixel))*(xm); \
    xB += qBlue((pixel))*(xm); \
    yR += qRed((pixel))*(ym); yG += qGreen((pixel))*(ym); \
    yB += qBlue((pixel))*(ym);

QImage Blitz::edge(QImage &img)
{

    int x, y, w = img.width(), h = img.height();
    if(w < 3 || h < 3){
        qWarning("Blitz::edge(): Image is too small!");
        return(img);
    }
    if(img.isNull())
        return(img);

    if(img.depth() != 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    else if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);

    QImage buffer(w, h, QImage::Format_RGB32);
    QRgb *dest;
    QRgb *s, *scanblock[3];


#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX sobel edge
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::MMX)){
        int xmatrix[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
        int ymatrix[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
        int i, *xm, *ym;

        for(y=0; y < h; ++y){
            scanblock[1] = (QRgb *)img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            if(y == 0){
                scanblock[0] = (QRgb *)img.scanLine(y);
                scanblock[2] = (QRgb *)img.scanLine(y+1);
            }
            else if(y == h-1){
                scanblock[0] = (QRgb *)img.scanLine(y-1);
                scanblock[2]  = (QRgb *)img.scanLine(y);
            }
            else{
                scanblock[0] = (QRgb *)img.scanLine(y-1);
                scanblock[2] = (QRgb *)img.scanLine(y+1);
            }
            //
            // x == 0, double over first pixel
            //
            __asm__ __volatile__
                ("pxor %%mm7, %%mm7\n\t" // used for unpacking
                 "pxor %%mm5, %%mm5\n\t" // clear accumulator
                 "pxor %%mm6, %%mm6\n\t" // ""
                 : : );
            for(i=0, xm=xmatrix, ym=ymatrix; i < 3; ++i, xm+=3, ym+=3){
                s = scanblock[i];
                __asm__ __volatile__
                    (// first pixel
                     "movd (%0), %%mm0\n\t" // load pixel into mm0
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                     "movq %%mm0, %%mm4\n\t" // and mm4 since we are doubling over
                     "movd 0(%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd 0(%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     // second pixel (doubled over)
                     "movq %%mm4, %%mm1\n\t" // copy saved pixel to mm1
                     "movd 4(%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd 4(%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm4\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm4, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     // third pixel
                     "movd 4(%0), %%mm0\n\t" // load pixel into mm0
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                     "movd 8(%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd 8(%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     : : "r"(s), "r"(xm), "r"(ym));
            }
            __asm__ __volatile__
                (// calculate abs, sum, and write
                 "movq %%mm5, %%mm0\n\t" // calculate abs of x accumulator
                 "psraw $15, %%mm0\n\t"
                 "pxor %%mm0, %%mm5\n\t"
                 "psubw %%mm0, %%mm5\n\t"
                 "movq %%mm6, %%mm0\n\t" // calculate abs of y accumulator
                 "psraw $15, %%mm0\n\t"
                 "pxor %%mm0, %%mm6\n\t"
                 "psubw %%mm0, %%mm6\n\t"
                 "paddw %%mm5, %%mm6\n\t" // add together
                 "packuswb %%mm6, %%mm6\n\t" // and write
                 "movd %%mm6, (%0)\n\t"
                 : : "r"(dest));
            dest++;

            //
            // Now x == 1, process middle of image
            //

            for(x=1; x < w-1; ++x){
                __asm__ __volatile__
                    ("pxor %%mm5, %%mm5\n\t" // clear accumulator
                     "pxor %%mm6, %%mm6\n\t"
                     : : );
                for(i=0, xm=xmatrix, ym=ymatrix; i < 3; ++i, xm+=3, ym+=3){
                    s = scanblock[i];
                    __asm__ __volatile__
                        (// first pixel
                         "movd (%0), %%mm0\n\t" // load pixel into mm0
                         "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                         "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                         "movd (%1), %%mm2\n\t" // load x matrix into mm2
                         "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                         "packssdw %%mm2, %%mm2\n\t"
                         "movd (%2), %%mm3\n\t" // load y matrix into mm3
                         "punpckldq %%mm3, %%mm3\n\t" // expand
                         "packssdw %%mm3, %%mm3\n\t"
                         "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                         "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                         "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                         "paddw %%mm1, %%mm6\n\t"
                         // second pixel
                         "movd 4(%0), %%mm0\n\t" // load pixel into mm0
                         "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                         "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                         "movd 4(%1), %%mm2\n\t" // load x matrix into mm2
                         "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                         "packssdw %%mm2, %%mm2\n\t"
                         "movd 4(%2), %%mm3\n\t" // load y matrix into mm3
                         "punpckldq %%mm3, %%mm3\n\t" // expand
                         "packssdw %%mm3, %%mm3\n\t"
                         "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                         "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                         "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                         "paddw %%mm1, %%mm6\n\t"
                         // third pixel
                         "movd 8(%0), %%mm0\n\t" // load pixel into mm0
                         "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                         "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                         "movd 8(%1), %%mm2\n\t" // load x matrix into mm2
                         "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                         "packssdw %%mm2, %%mm2\n\t"
                         "movd 8(%2), %%mm3\n\t" // load y matrix into mm3
                         "punpckldq %%mm3, %%mm3\n\t" // expand
                         "packssdw %%mm3, %%mm3\n\t"
                         "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                         "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                         "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                         "paddw %%mm1, %%mm6\n\t"
                         : : "r"(s), "r"(xm), "r"(ym));
                }
                __asm__ __volatile__
                    (// calculate abs, sum, and write
                     "movq %%mm5, %%mm0\n\t" // calculate abs of x accumulator
                     "psraw $15, %%mm0\n\t"
                     "pxor %%mm0, %%mm5\n\t"
                     "psubw %%mm0, %%mm5\n\t"
                     "movq %%mm6, %%mm0\n\t" // calculate abs of y accumulator
                     "psraw $15, %%mm0\n\t"
                     "pxor %%mm0, %%mm6\n\t"
                     "psubw %%mm0, %%mm6\n\t"
                     "paddw %%mm5, %%mm6\n\t" // add together
                     "packuswb %%mm6, %%mm6\n\t" // and write
                     "movd %%mm6, (%0)\n\t"
                     : : "r"(dest));
                dest++;
                ++scanblock[0], ++scanblock[1], ++scanblock[2];
            }

            //
            // x = w-1, double over last pixel
            //

            __asm__ __volatile__
                ("pxor %%mm5, %%mm5\n\t" // clear accumulator
                 "pxor %%mm6, %%mm6\n\t"
                 : : );
            for(i=0, xm=xmatrix, ym=ymatrix; i < 3; ++i, xm+=3, ym+=3){
                s = scanblock[i];
                __asm__ __volatile__
                    (// first pixel
                     "movd (%0), %%mm0\n\t" // load pixel into mm0
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                     "movd (%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd (%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     // second pixel
                     "movd 4(%0), %%mm0\n\t" // load pixel into mm0
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                     "movd 4(%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd 4(%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     // third pixel
                     "movd 4(%0), %%mm0\n\t" // load pixel into mm0
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "movq %%mm0, %%mm1\n\t" // copy pixel to mm1
                     "movd 8(%1), %%mm2\n\t" // load x matrix into mm2
                     "punpckldq %%mm2, %%mm2\n\t" // expand to all words
                     "packssdw %%mm2, %%mm2\n\t"
                     "movd 8(%2), %%mm3\n\t" // load y matrix into mm3
                     "punpckldq %%mm3, %%mm3\n\t" // expand
                     "packssdw %%mm3, %%mm3\n\t"
                     "pmullw %%mm2, %%mm0\n\t" // multiply pixel w/ x matrix
                     "pmullw %%mm3, %%mm1\n\t" // and multiply copy w/ y matrix
                     "paddw %%mm0, %%mm5\n\t"  // add to accumulators
                     "paddw %%mm1, %%mm6\n\t"
                     : : "r"(s), "r"(xm), "r"(ym));
            }
            __asm__ __volatile__
                (// calculate abs, sum, and write
                 "movq %%mm5, %%mm0\n\t" // calculate abs of x accumulator
                 "psraw $15, %%mm0\n\t"
                 "pxor %%mm0, %%mm5\n\t"
                 "psubw %%mm0, %%mm5\n\t"
                 "movq %%mm6, %%mm0\n\t" // calculate abs of y accumulator
                 "psraw $15, %%mm0\n\t"
                 "pxor %%mm0, %%mm6\n\t"
                 "psubw %%mm0, %%mm6\n\t"
                 "paddw %%mm5, %%mm6\n\t" // add together
                 "packuswb %%mm6, %%mm6\n\t" // and write
                 "movd %%mm6, (%0)\n\t"
                 : : "r"(dest));
            dest++;
        }
        __asm__ __volatile__ ("emms\n\t" : :);
    }
    else
#endif
    {
        int xR, xG, xB, yR, yG, yB;
        for(y=0; y < h; ++y){
            scanblock[1] = (QRgb *)img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            if(y == 0){
                scanblock[0] = (QRgb *)img.scanLine(y);
                scanblock[2] = (QRgb *)img.scanLine(y+1);
            }
            else if(y == h-1){
                scanblock[0] = (QRgb *)img.scanLine(y-1);
                scanblock[2]  = (QRgb *)img.scanLine(y);
            }
            else{
                scanblock[0] = (QRgb *)img.scanLine(y-1);
                scanblock[2] = (QRgb *)img.scanLine(y+1);
            }

            // x == 0, double over first pixel
            xR = xG = xB = yR = yG = yB = 0;
            s = scanblock[0];
            SOBEL(-1, 1, *s); SOBEL(0, 2, *s); ++s; SOBEL(1, 1, *s);
            s = scanblock[1];
            SOBEL(-2, 0, *s); SOBEL(0, 0, *s); ++s; SOBEL(2, 0, *s);
            s = scanblock[2];
            SOBEL(-1, -1, *s); SOBEL(0, -2, *s); ++s; SOBEL(1, -1, *s);
            xR = qAbs(xR)+qAbs(yR); xG = qAbs(xG)+qAbs(yG);
            xB = qAbs(xB)+qAbs(yB);
            *dest++ = qRgb(qMin(xR, 255), qMin(xG, 255), qMin(xB, 255));

            // x == 1, process middle of image
            for(x=1; x < w-1; ++x){
                xR = xG = xB = yR = yG = yB = 0;
                s = scanblock[0];
                SOBEL(-1, 1, *s); ++s; SOBEL(0, 2, *s); ++s; SOBEL(1, 1, *s);
                s = scanblock[1];
                SOBEL(-2, 0, *s); ++s; SOBEL(0, 0, *s); ++s; SOBEL(2, 0, *s);
                s = scanblock[2];
                SOBEL(-1, -1, *s); ++s; SOBEL(0, -2, *s); ++s; SOBEL(1, -1, *s);
                ++scanblock[0]; ++scanblock[1]; ++scanblock[2];
                xR = qAbs(xR)+qAbs(yR); xG = qAbs(xG)+qAbs(yG);
                xB = qAbs(xB)+qAbs(yB);
                *dest++ = qRgb(qMin(xR, 255), qMin(xG, 255), qMin(xB, 255));
            }

            // x == w-1, double over last pixel
            xR = xG = xB = yR = yG = yB = 0;
            s = scanblock[0];
            SOBEL(-1, 1, *s); ++s; SOBEL(0, 2, *s); SOBEL(1, 1, *s);
            s = scanblock[1];
            SOBEL(-2, 0, *s); ++s; SOBEL(0, 0, *s); SOBEL(2, 0, *s);
            s = scanblock[2];
            SOBEL(-1, -1, *s); ++s; SOBEL(0, -2, *s); SOBEL(1, -1, *s);
            xR = qAbs(xR)+qAbs(yR); xG = qAbs(xG)+qAbs(yG);
            xB = qAbs(xB)+qAbs(yB);
            *dest++ = qRgb(qMin(xR, 255), qMin(xG, 255), qMin(xB, 255));
        }
    }
    return(buffer);
}

QImage Blitz::charcoal(QImage &img)
{
    QImage buffer(edge(img));
    buffer = blur(buffer, 1);
    normalize(buffer);
    buffer.invertPixels();
    grayscale(buffer, true);
    return(buffer);
}

QImage Blitz::swirl(QImage &img, float degrees)
{
    float sine, cosine, distance, radius, factor;
    float x_center, x_distance, x_scale;
    float y_center, y_distance, y_scale;
    int x, y, w, h;
    QRgb *dest;

    w = img.width();
    h = img.height();

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QImage buffer(w, h, img.hasAlphaChannel() ?
                  QImage::Format_ARGB32 : QImage::Format_RGB32);

    x_center = w/2.0;
    y_center = h/2.0;
    radius = qMax(x_center, y_center);
    x_scale = y_scale = 1.0;
    if(w > h)
        y_scale = (float)(w/h);
    else if(w < h)
        x_scale = (float)(h/w);
    degrees = (M_PI*degrees)/180.0;

    InlineInterpolate interpolate(&img, 0);
    if(img.depth() > 8){
        QRgb *src;
        for(y=0; y < h; ++y){
            src = (QRgb *)img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            y_distance = y_scale*(y-y_center);
            for(x=0; x < w; ++x){
                x_distance = x_scale*(x-x_center);
                distance = x_distance*x_distance + y_distance*y_distance;
                if(distance >= (radius*radius))
                    *dest = src[x];
                else{
                    factor = 1.0-std::sqrt(distance)/radius;
                    sine = std::sin(degrees*factor*factor);
                    cosine = std::cos(degrees*factor*factor);
                    *dest = interpolate.
                        interpolate((cosine*x_distance-sine*y_distance)/
                                    x_scale+x_center,
                                    (sine*x_distance+cosine*y_distance)/
                                    y_scale+y_center);
                }
                ++dest;
            }
        }
    }
    else{
        QVector<QRgb> cTable(img.colorTable());
        unsigned char *src;
        for(y=0; y < h; ++y){
            src = img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            y_distance = y_scale*(y-y_center);
            for(x=0; x < w; ++x){
                x_distance = x_scale*(x-x_center);
                distance = x_distance*x_distance + y_distance*y_distance;
                if(distance >= (radius*radius))
                    *dest = cTable.at(src[x]);
                else{
                    factor = 1.0-std::sqrt(distance)/radius;
                    sine = std::sin(degrees*factor*factor);
                    cosine = std::cos(degrees*factor*factor);
                    *dest = interpolate.
                        interpolate((cosine*x_distance-sine*y_distance)/
                                    x_scale+x_center,
                                    (sine*x_distance+cosine*y_distance)/
                                    y_scale+y_center);
                }
                ++dest;
            }
        }
    }
    return(buffer);
}

QImage Blitz::implode(QImage &img, float amount)
{
    float distance, radius, factor;
    float x_center, x_distance, x_scale;
    float y_center, y_distance, y_scale;
    int x, y, w, h;
    QRgb *dest;

    w = img.width();
    h = img.height();

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QImage buffer(w, h, img.hasAlphaChannel() ?
                  QImage::Format_ARGB32 : QImage::Format_RGB32);

    x_scale = y_scale = 1.0;
    x_center = 0.5*w;
    y_center = 0.5*h;
    radius = x_center;
    if(w > h)
        y_scale = (float)(w/h);
    else if(w < h){
        x_scale = (float)(h/w);
        radius = y_center;
    }

    InlineInterpolate interpolate(&img, 0);
    if(img.depth() > 8){
        QRgb *src;
        for(y=0; y < h; ++y){
            src = (QRgb *)img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            y_distance = y_scale*(y-y_center);
            for(x=0; x < w; ++x){
                x_distance = x_scale*(x-x_center);
                distance = x_distance*x_distance + y_distance*y_distance;
                if(distance >= (radius*radius))
                    *dest = src[x];
                else{
                    factor = 1.0;
                    if(distance > 0.0)
                        factor = std::pow(std::sin(((float)M_PI)*
                                                   std::sqrt(distance)/
                                                   radius/2), -amount);
                    *dest = interpolate.
                        interpolate(factor*x_distance/x_scale+x_center,
                                    factor*y_distance/y_scale+y_center);
                }
                ++dest;
            }
        }
    }
    else{
        QVector<QRgb> cTable(img.colorTable());
        unsigned char *src;
        for(y=0; y < h; ++y){
            src = img.scanLine(y);
            dest = (QRgb *)buffer.scanLine(y);
            y_distance = y_scale*(y-y_center);
            for(x=0; x < w; ++x){
                x_distance = x_scale*(x-x_center);
                distance = x_distance*x_distance + y_distance*y_distance;
                if(distance >= (radius*radius))
                    *dest = cTable.at(src[x]);
                else{
                    factor = 1.0;
                    if(distance > 0.0)
                        factor = std::pow(std::sin(((float)M_PI)*
                                                   std::sqrt(distance)/
                                                   radius/2), -amount);
                    *dest = interpolate.
                        interpolate(factor*x_distance/x_scale+x_center,
                                    factor*y_distance/y_scale+y_center);
                }
                ++dest;
            }
        }
    }
    return(buffer);
}

QImage Blitz::wave(QImage &img, float amplitude, float length,
                   unsigned int background)
{
    int x, y, w, h;
    QRgb *dest;
    float *sine_map;

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QImage buffer(img.width(), (int)(img.height()+2.0*std::abs(amplitude)),
                  QImage::Format_RGB32);
    w = buffer.width();
    h = buffer.height();

    sine_map = new float[w];
    for(x=0; x < w; ++x)
        sine_map[x] = std::abs(amplitude)+amplitude*std::sin((2*M_PI*x)/length);

    InlineInterpolate interpolate(&img, background);
    for(y=0; y < h; ++y){
        dest = (QRgb *)buffer.scanLine(y);
        for(x=0; x < w; ++x)
            *dest++ = interpolate.
                interpolateBackground(x, y-sine_map[x]);
    }
    delete[] sine_map;
    return(buffer);
}

QImage& Blitz::modulate(QImage &img, QImage &modImg, bool reverse,
                        ModulationType type, int factor, RGBChannel channel)
{
    if(img.isNull() || modImg.isNull())
        return(img);

    // for image, we handle only depth 32
    if(img.depth() < 32 || img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);

    // for modImage, we handle depth 8 and 32
    if(modImg.depth() < 8)
        modImg = modImg.convertToFormat(QImage::Format_Indexed8);
    else if(modImg.format() == QImage::Format_ARGB32_Premultiplied)
        modImg = modImg.convertToFormat(QImage::Format_ARGB32);

    unsigned int x1 = img.width(),  y1 = img.height();
    unsigned int x2 = modImg.width(), y2 = modImg.height();
    unsigned int x, y;
    unsigned int *src, *modulation_colorTable;
    unsigned char *modulation_src;
    int r, g, b, mod = 0;
    QRgb color1, color2;
    InlineHSV hsv;

    QVector<QRgb> cTable;
    if(modImg.format() == QImage::Format_Indexed8){
        cTable = modImg.colorTable();
        modulation_colorTable = cTable.data();
    }
    else
        modulation_colorTable = NULL;

    for(y=0; y < y1; ++y){
        src = (QRgb *)img.scanLine(y);
        modulation_src = modImg.scanLine(y%y2);
        x=0;
        while(x < x1){
            color2 = (!modulation_colorTable) ?
                *((QRgb *)modulation_src) :
                modulation_colorTable[*modulation_src];
            if(reverse){
                color1 = color2;
                color2 = *src;
            }
            else
                color1 = *src;

            if(type == Intensity || type == Contrast){
                r = qRed(color1); g = qGreen(color1); b = qBlue(color1);
                if(channel != All){
                    mod = (channel == Red) ? qRed(color2) :
                        (channel == Green) ? qGreen(color2) :
                        (channel == Blue) ? qBlue(color2) :
                        (channel == Grayscale) ? qGray(color2) : 0;
                    mod = mod*factor/50;
                }

                if(type == Intensity){
                    if(channel == All){
                        r += r * factor/50 * qRed(color2)/256;
                        g += g * factor/50 * qGreen(color2)/256;
                        b += b * factor/50 * qBlue(color2)/256;
                    }
                    else{
                        r += r * mod/256;
                        g += g * mod/256;
                        b += b * mod/256;
                    }
                }
                else{ // Contrast
                    if(channel == All){
                        r += (r-128) * factor/50 * qRed(color2)/128;
                        g += (g-128) * factor/50 * qGreen(color2)/128;
                        b += (b-128) * factor/50 * qBlue(color2)/128;
                    }
                    else{
                        r += (r-128) * mod/128;
                        g += (g-128) * mod/128;
                        b += (b-128) * mod/128;
                    }
                }
                *src = qRgba(qBound(0, r, 255), qBound(0, g, 255),
                             qBound(0, b, 255), qAlpha(*src));
            }
            else if(type == Saturation || type == HueShift){
                mod = (channel == Red) ? qRed(color2) :
                    (channel == Green) ? qGreen(color2) :
                    (channel == Blue) ? qBlue(color2) :
                    (channel == Grayscale||channel == All) ? qGray(color2) : 0;
                mod = mod*factor/50;
                hsv.convertRGB2HSV(color1);
                if(type == Saturation)
                    hsv.setSaturation(qBound(0, hsv.saturation()-
                                             hsv.saturation()*mod/256, 255));
                else{
                    int h = hsv.hue() + mod;
                    while(h<0) h+=360;
                    hsv.setHue(h %= 360);
                }
                hsv.convertHSV2RGB();
                *src = qRgba(hsv.red(), hsv.green(), hsv.blue(), qAlpha(*src));
            }
            ++src; ++x;
            modulation_src += (!modulation_colorTable) ? 4 : 1;

            if((x%x2) == 0)
                modulation_src -= (!modulation_colorTable) ? x2*4 : x2;
        }
    }
    return(img);
}


