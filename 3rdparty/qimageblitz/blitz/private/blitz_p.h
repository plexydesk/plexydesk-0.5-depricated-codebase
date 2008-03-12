#ifndef __BLITZ_PRIVATE_H
#define __BLITZ_PRIVATE_H

/* 
 Copyright (C) 1998, 1999, 2001, 2002, 2004, 2005, 2007
      Daniel M. Duley <daniel.duley@verizon.net>

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

#include <QColor>

class BlitzPrivate
{
public:
    static inline QRgb convertFromPremult(QRgb);
    static inline QRgb convertToPremult(QRgb);
    static inline QRgb byteMul(QRgb x, unsigned int a);
    static inline QRgb interpolate255(QRgb x, unsigned int a,
                                      QRgb y, unsigned int b);
    static inline QRgb setAlpha(QRgb rgb, unsigned int a);
    static inline int brightness(QRgb rgb);
    static inline void hull(int x_offset, int y_offset, int w, int h,
                            unsigned char *f, unsigned char *g, int polarity);
    static int defaultConvolveMatrixSize(float radius, float sigma,
                                         bool quality);
    static float* getBlurKernel(int &kernel_width, float sigma);
    static void blurScanLine(float *kernel, int kern_width,
                             QRgb *source, QRgb *destination,
                             int columns, int offset);
};

inline QRgb BlitzPrivate::byteMul(QRgb x, unsigned int a)
{
    unsigned int t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return(x);
}

inline QRgb BlitzPrivate::interpolate255(QRgb x, unsigned int a,
                                         QRgb y, unsigned int b)
{
    unsigned int t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return(x);
}

inline QRgb BlitzPrivate::convertFromPremult(QRgb p)
{
    int alpha = qAlpha(p);
    return(!alpha ? 0 : qRgba(255*qRed(p)/alpha,
                              255*qGreen(p)/alpha,
                              255*qBlue(p)/alpha,
                              alpha));
}

inline QRgb BlitzPrivate::convertToPremult(QRgb p)
{
    unsigned int a = p >> 24;
    unsigned int t = (p & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    p = ((p >> 8) & 0xff) * a;
    p = (p + ((p >> 8) & 0xff) + 0x80);
    p &= 0xff00;
    p |= t | (a << 24);
    return(p);
}

inline void BlitzPrivate::hull(int x_offset, int y_offset, int w, int h,
                               unsigned char *f, unsigned char *g,
                               int polarity)
{
    int x, y, v;
    unsigned char *p, *q, *r, *s;
    p = f+(w+2); q = g+(w+2);
    r = p+(y_offset*(w+2)+x_offset);
    for(y=0; y < h; ++y, ++p, ++q, ++r){
        ++p; ++q; ++r;
        if(polarity > 0){
            for(x=w; x > 0; --x, ++p, ++q, ++r){
                v = (*p);
                if((int)*r >= (v+2)) v += 1;
                *q = (unsigned char)v;
            }
        }
        else{
            for(x=w; x > 0; --x, ++p, ++q, ++r){
                v = (*p);
                if((int)*r <= (v-2)) v -= 1;
                *q = (unsigned char)v;
            }
        }
    }
    p = f+(w+2); q = g+(w+2);
    r = q+(y_offset*(w+2)+x_offset); s = q-(y_offset*(w+2)+x_offset);
    for(y=0; y < h; ++y, ++p, ++q, ++r, ++s){
        ++p; ++q; ++r; ++s;
        if(polarity > 0){
            for(x=w; x > 0; --x, ++p, ++q, ++r, ++s){
                v = (*q);
                if(((int)*s >= (v+2)) && ((int)*r > v)) v+=1;
                *p = (unsigned char)v;
            }
        }
        else{
            for(x=w; x > 0; --x, ++p, ++q, ++r, ++s){
                v = (int)(*q);
                if (((int)*s <= (v-2)) && ((int)*r < v)) v -= 1;
                *p = (unsigned char)v;
            }
        }
    }
}

inline int BlitzPrivate::brightness(QRgb rgb)
{
    return(qMax(qRed((rgb)), qMax(qBlue((rgb)), qGreen((rgb)))));
}

inline QRgb BlitzPrivate::setAlpha(QRgb rgb, unsigned int a)
{
    return(((a & 0xFF) << 24) | (rgb & 0x00FFFFFF));
}

#endif

