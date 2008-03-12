#include <config-processor.h>
#include <config-externalasm.h>
#include "blitzcpu.h"
#include "qimageblitz.h"
#include <string.h>
#include <stdlib.h>

/*
 * Copyright (C) 2004, 2005, 2007 Daniel M. Duley <daniel.duley@verizon.net>
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
 *
 */

/* OTHER CREDITS:
 *
 * This is the normal smoothscale method, based on Imlib2's smoothscale.
 *
 * Originally I took the algorithm used in NetPBM and Qt and added MMX/3dnow
 * optimizations. It ran in about 1/2 the time as Qt. Then I ported Imlib's
 * C algorithm and it ran at about the same speed as my MMX optimized one...
 * Finally I ported Imlib's MMX version and it ran in less than half the
 * time as my MMX algorithm, (taking only a quarter of the time Qt did).
 * After further optimization it seems to run at around 1/6th.
 *
 * Changes include formatting, namespaces and other C++'ings, removal of old
 * #ifdef'ed code, and removal of unneeded border calculation code.
 *
 * Imlib2 is (C) Carsten Haitzler and various contributors. The MMX code
 * is by Willem Monsuwe <willem@stack.nl>. All other modifications are
 * (C) Daniel M. Duley.
 */

#if defined(__i386__) && ( defined(__GNUC__) || defined(__INTEL_COMPILER) )
#  if defined(HAVE_MMX) && defined(HAVE_EXTERNAL_ASM)
#    define USE_MMX_INLINE_ASM
#  endif
#endif

namespace QImageScale{
    typedef struct __qimage_scale_info
    {
        int *xpoints;
        unsigned int **ypoints;
        int *xapoints, *yapoints;
        int xup_yup;
    } QImageScaleInfo;

    unsigned int** qimageCalcYPoints(unsigned int *src, int sw, int sh,
                                     int dh);
    int* qimageCalcXPoints(int sw, int dw);
    int* qimageCalcApoints(int s, int d, int up);
    QImageScaleInfo* qimageFreeScaleInfo(QImageScaleInfo *isi);
    QImageScaleInfo *qimageCalcScaleInfo(const QImage &img, int sw, int sh,
                                         int dw, int dh, char aa);
}

#ifdef USE_MMX_INLINE_ASM
extern "C" {
    void __qimageScale_mmx_AARGBA(QImageScale::QImageScaleInfo *isi,
                                  unsigned int *dest, int dxx, int dyy,
                                  int dx, int dy, int dw, int dh,
                                  int dow, int sow);
}
#endif

using namespace QImageScale;

QImage Blitz::smoothScale(QImage &src, int w, int h,
                          Qt::AspectRatioMode aspectRatio)
{
    return(smoothScale(src, QSize(w, h), aspectRatio));
}

QImage Blitz::smoothScale(QImage &src, const QSize &sz,
                          Qt::AspectRatioMode aspectRatio)
{
#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX smoothscaling
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::MMX)){
        QImage buffer;
        QSize destSize(src.size());
        destSize.scale(sz, aspectRatio);
        if(src.isNull() || !destSize.isValid())
            return(buffer);
        if(src.depth() != 32){
            src = src.convertToFormat(src.hasAlphaChannel() ?
                                      QImage::Format_ARGB32 :
                                      QImage::Format_RGB32);
        }
        else if(src.format() == QImage::Format_ARGB32_Premultiplied)
            src = src.convertToFormat(QImage::Format_ARGB32);

        QImageScaleInfo *scaleinfo =
            qimageCalcScaleInfo(src, src.width(), src.height(),
                                destSize.width(), destSize.height(), true);
        if(!scaleinfo)
            return(buffer);

        buffer = QImage(destSize, src.format());
        __qimageScale_mmx_AARGBA(scaleinfo, (unsigned int *)buffer.scanLine(0), 0, 0,
                                 0, 0, destSize.width(), destSize.height(),
                                 destSize.width(), src.width());
        qimageFreeScaleInfo(scaleinfo);
        return(buffer);
    }
    else
#endif
    {
        return(src.scaled(sz, aspectRatio, Qt::SmoothTransformation));
    }
}

//
// Code ported from Imlib...
//

// FIXME: replace with qRed, etc... These work on pointers to pixels, not
// pixel values
#define A_VAL(p) ((unsigned char *)(p))[3]
#define R_VAL(p) ((unsigned char *)(p))[2]
#define G_VAL(p) ((unsigned char *)(p))[1]
#define B_VAL(p) ((unsigned char *)(p))[0]

#define INV_XAP                   (256 - xapoints[x])
#define XAP                       (xapoints[x])
#define INV_YAP                   (256 - yapoints[dyy + y])
#define YAP                       (yapoints[dyy + y])

unsigned int** QImageScale::qimageCalcYPoints(unsigned int *src,
                                              int sw, int sh, int dh)
{
    unsigned int **p;
    int i, j = 0;
    int val, inc, rv = 0;

    if(dh < 0){
        dh = -dh;
        rv = 1;
    }
    p = new unsigned int* [dh+1];

    val = 0;
    inc = (sh << 16) / dh;
    for(i = 0; i < dh; i++){
        p[j++] = src + ((val >> 16) * sw);
        val += inc;
    }
    if(rv){
        for(i = dh / 2; --i >= 0; ){
            unsigned int *tmp = p[i];
            p[i] = p[dh - i - 1];
            p[dh - i - 1] = tmp;
        }
    }
    return(p);
}

int* QImageScale::qimageCalcXPoints(int sw, int dw)
{
    int *p, i, j = 0;
    int val, inc, rv = 0;

    if(dw < 0){
        dw = -dw;
        rv = 1;
    }
    p = new int[dw+1];

    val = 0;
    inc = (sw << 16) / dw;
    for(i = 0; i < dw; i++){
        p[j++] = (val >> 16);
        val += inc;
    }

    if(rv){
        for(i = dw / 2; --i >= 0; ){
            int tmp = p[i];
            p[i] = p[dw - i - 1];
            p[dw - i - 1] = tmp;
        }
    }
   return(p);
}

int* QImageScale::qimageCalcApoints(int s, int d, int up)
{
    int *p, i, j = 0, rv = 0;

    if(d < 0){
        rv = 1;
        d = -d;
    }
    p = new int[d];

    /* scaling up */
    if(up){
        int val, inc;

        val = 0;
        inc = (s << 16) / d;
        for(i = 0; i < d; i++){
            p[j++] = (val >> 8) - ((val >> 8) & 0xffffff00);
            if((val >> 16) >= (s - 1))
                p[j - 1] = 0;
            val += inc;
        }
    }
    /* scaling down */
    else{
        int val, inc, ap, Cp;
        val = 0;
        inc = (s << 16) / d;
        Cp = ((d << 14) / s) + 1;
        for(i = 0; i < d; i++){
            ap = ((0x100 - ((val >> 8) & 0xff)) * Cp) >> 8;
            p[j] = ap | (Cp << 16);
            j++;
            val += inc;
        }
    }
    if(rv){
        int tmp;
        for(i = d / 2; --i >= 0; ){
            tmp = p[i];
            p[i] = p[d - i - 1];
            p[d - i - 1] = tmp;
        }
    }
    return(p);
}

QImageScaleInfo* QImageScale::qimageFreeScaleInfo(QImageScaleInfo *isi)
{
    if(isi){
        delete[] isi->xpoints;
        delete[] isi->ypoints;
        delete[] isi->xapoints;
        delete[] isi->yapoints;
        delete isi;
    }
    return(NULL);
}

QImageScaleInfo* QImageScale::qimageCalcScaleInfo(const QImage &img,
                                                  int sw, int sh,
                                                  int dw, int dh, char aa)
{
    QImageScaleInfo *isi;
    int scw, sch;

    scw = dw * img.width() / sw;
    sch = dh * img.height() / sh;

    isi = new QImageScaleInfo;
    if(!isi)
        return(NULL);
    memset(isi, 0, sizeof(QImageScaleInfo));

    isi->xup_yup = (qAbs(dw) >= sw) + ((qAbs(dh) >= sh) << 1);

    isi->xpoints = qimageCalcXPoints(img.width(), scw);
    if(!isi->xpoints)
        return(qimageFreeScaleInfo(isi));
    isi->ypoints = qimageCalcYPoints((unsigned int *)img.scanLine(0),
                                     img.width(), img.height(), sch);
    if (!isi->ypoints)
        return(qimageFreeScaleInfo(isi));
    if(aa){
        isi->xapoints = qimageCalcApoints(img.width(), scw, isi->xup_yup & 1);
        if(!isi->xapoints)
            return(qimageFreeScaleInfo(isi));
        isi->yapoints = qimageCalcApoints(img.height(), sch, isi->xup_yup & 2);
        if(!isi->yapoints)
            return(qimageFreeScaleInfo(isi));
    }
    return(isi);
}

