/*
 Copyright (C) 1998, 1999, 2001, 2002, 2004, 2005, 2007
      Daniel M. Duley <daniel.duley@verizon.net>
 (C) 2000 Josef Weidendorfer <weidendo@in.tum.de>
 (C) 1999 Geert Jansen <g.t.jansen@stud.tue.nl>

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

#include "qimageblitz.h"
#include <config-processor.h>
#include "private/inlinehsv.h"
#include "private/blitz_p.h"
#include "blitzcpu.h"
#include <QVector>

#if defined(__i386__) && ( defined(__GNUC__) || defined(__INTEL_COMPILER) )
#  if defined(HAVE_MMX )
#    define USE_MMX_INLINE_ASM
#  endif
#endif

/**
 * Precalculated increment values for HSV contrast. Saves some expensive
 * floating point math. These are half the integer results of:
 *
 *  alpha*(alpha*(sin(M_PI*(brightness-alpha))+1.0)-brightness)
 *
 * where alpha is 0.5+M_EPSILON and brightness is the percentage for values
 * ranging from 0 to 255, (Qt compatible HSV brightness values). The other
 * half of the lookup table is the inverse of these values. I am so clever :)
 * (mosfet)
 */
static char contrast_table[128] = {
    0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 8, 9, 9,
    10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17,
    17, 18, 18, 19, 19, 19, 20, 20, 21, 21, 21, 22, 22, 22, 23,
    23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26,
    26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 24, 24, 24,
    23, 23, 23, 22, 22, 21, 21, 20, 20, 20, 19, 18, 18, 17, 17,
    16, 16, 15, 14, 14, 13, 12, 11, 11, 10, 9, 8, 7, 7, 6, 5,
    4, 3, 2, 1, 0
};

bool Blitz::grayscale(QImage &img, bool reduceDepth)
{
    if(img.isNull())
        return(false);

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX grayscale
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::IntegerSSE)){
        MMX::Pack4 packedmult = {{5, 16, 11, 1}};
        MMX::Pack4 alphamask = {{0x0000, 0x0000, 0x0000, 0xFFFF}};
        MMX::Pack4 noalphamask = {{0xFFFF, 0xFFFF, 0xFFFF, 0x0000}};

        if(reduceDepth && !img.hasAlphaChannel()){
            int y, w = img.width(), h = img.height();
            QImage buffer(w, h, QImage::Format_Indexed8);

            QVector<QRgb> cTable(256);
            unsigned int *src = cTable.data();
            for(y=0; y < 256; ++y)
                *src++ = qRgb(y, y, y);
            buffer.setColorTable(cTable);

            src = (unsigned int *)img.scanLine(0);
            unsigned char *end, *dest;

            __asm__ __volatile__
                ("pxor %%mm7, %%mm7\n\t"
                 "movq (%0), %%mm4\n\t"
                 "movq (%1), %%mm5\n\t"
                 "movq (%2), %%mm6\n\t"
                 : : "r"(&packedmult), "r"(&alphamask), "r"(&noalphamask));
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                end = dest+w;
                while(dest != end){
                    __asm__ __volatile__
                        ("movd (%0), %%mm0\n\t" // load pixel
                         "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                         "pand %%mm6, %%mm0\n\t" // zero alpha so we can use pmaddwd

                         "pmaddwd %%mm4, %%mm0\n\t" // 2 multiplies and sum BG, RA
                         "pshufw $0xE4, %%mm0, %%mm1\n\t"
                         "punpckhwd %%mm7, %%mm1\n\t" // sum R
                         "paddw %%mm1, %%mm0\n\t"
                         "psrlw $0x05, %%mm0\n\t" // divide by 32
                         "movd %%mm0, %%eax\n\t"
                         "movb %%al, (%1)\n\t"
                         : : "r"(src), "r"(dest) : "%eax");
                    ++src;
                    ++dest;
                }
            }
            __asm__ __volatile__ ("emms\n\t" : :);
            img = buffer;
        }
        else{
            // 8bpp or 32bpp w/ no conversion
            int count = img.depth() > 8 ? img.width()*img.height() : img.numColors();

            QVector<QRgb> cTable;
            if(img.depth() == 8)
                cTable = img.colorTable();

            unsigned int *data = (img.depth() > 8) ?
                (unsigned int *)img.scanLine(0) : cTable.data();
            unsigned int *end = data+count;

            __asm__ __volatile__
                ("pxor %%mm7, %%mm7\n\t"
                 "movq (%0), %%mm4\n\t"
                 "movq (%1), %%mm5\n\t"
                 "movq (%2), %%mm6\n\t"
                 : : "r"(&packedmult), "r"(&alphamask), "r"(&noalphamask));
            while(data != end){
                __asm__ __volatile__
                    ("movd (%0), %%mm0\n\t" // load pixel
                     "punpcklbw %%mm7, %%mm0\n\t" // upgrade to quad
                     "pshufw $0xE4, %%mm0, %%mm2\n\t" // copy for alpha later
                     "pand %%mm6, %%mm0\n\t" // zero alpha so we can use pmaddwd

                     "pmaddwd %%mm4, %%mm0\n\t" // 2 multiplies and sum BG, RA
                     "pshufw $0xE4, %%mm0, %%mm1\n\t"
                     "punpckhwd %%mm7, %%mm1\n\t" // sum R
                     "paddw %%mm1, %%mm0\n\t"
                     "psrlw $0x05, %%mm0\n\t" // divide by 32

                     "pshufw $0x00, %%mm0, %%mm0\n\t" // copy to all pixels
                     "pand %%mm6, %%mm0\n\t" // reset original alpha
                     "pand %%mm5, %%mm2\n\t"
                     "por %%mm2, %%mm0\n\t"

                     "packuswb %%mm0, %%mm0\n\t" // pack and write
                     "movd %%mm0, (%0)\n\t"
                     : : "r"(data));
                ++data;
            }
            __asm__ __volatile__ ("emms\n\t" : :);
            if(img.depth() == 8)
                img.setColorTable(cTable);
        }
    }
    else
#endif
    {
        // Non MMX version
        if(reduceDepth && img.format() ==  QImage::Format_RGB32){
            // 32bpp w/ conversion to palette
            int y, w = img.width(), h = img.height();
            QImage buffer(w, h, QImage::Format_Indexed8);

            QVector<QRgb> cTable(256);
            unsigned int *src = cTable.data();
            for(y=0; y < 256; ++y)
                *src++ = qRgb(y, y, y);
            buffer.setColorTable(cTable);

            src = (unsigned int *)img.scanLine(0);
            unsigned char *end, *dest;
            unsigned int pixel;
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                end = dest+w;
                while(dest != end){
                    pixel = *src++;
                    *dest++ = qGray(qRed(pixel), qGreen(pixel), qBlue(pixel));
                }
            }
            img = buffer;
        }
        else{
            // 8bpp or 32bpp w/ no conversion
            int count = img.depth() > 8 ? img.width()*img.height() : img.numColors();

            QVector<QRgb> cTable;
            if(img.depth() == 8)
                cTable = img.colorTable();

            unsigned int *data = (img.depth() > 8) ?
                (unsigned int *)img.scanLine(0) : cTable.data();
            unsigned int *end = data+count;
            unsigned int pixel;
            unsigned char c;
            while(data != end){
                pixel = *data;
                c = qGray(qRed(pixel), qGreen(pixel), qBlue(pixel));
                *data++ = qRgba(c, c, c, qAlpha(pixel));
            }
            if(img.depth() == 8)
                img.setColorTable(cTable);
        }
    }
    return(true);
}

bool Blitz::invert(QImage &img, QImage::InvertMode mode)
{
    if(img.isNull())
        return(false);

#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX invert
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::MMX)){
        if(img.format() == QImage::Format_ARGB32_Premultiplied)
            img = img.convertToFormat(QImage::Format_ARGB32);
        else if(img.depth() < 8)
            img = img.convertToFormat(QImage::Format_Indexed8);

        unsigned int mask = (mode == QImage::InvertRgba ? 0xFFFFFFFF :
                             0x00FFFFFF);
        MMX::Pack2 packedmask = {{mask, mask}};

        int remainder, count;
        unsigned int *data, *end;
        QVector<QRgb> cTable;

        if(img.depth() <= 8){
            cTable = img.colorTable();
            remainder = img.numColors() % 4;
            count = img.numColors()-remainder;
            data = (unsigned int *)cTable.data();
        }
        else{
            remainder = (img.width()*img.height()) % 4;
            count = (img.width()*img.height())-remainder;
            data = (unsigned int *)img.scanLine(0);
        }
        end = data+count;

        __asm__ __volatile__ ("movq (%0), %%mm7\n\t" : : "r"(&packedmask));
        while(data != end){
            __asm__ __volatile__
                ("movq (%0), %%mm0\n\t"
                 "pxor %%mm7, %%mm0\n\t"
                 "movq %%mm0, (%0)\n\t"
                 : : "r"(data));
            data += 2;
        }
        end += remainder;
        while(data != end){
            __asm__ __volatile__
                ("movd (%0), %%mm0\n\t"
                 "pxor %%mm7, %%mm0\n\t"
                 "movd %%mm0, (%0)\n\t"
                 : : "r"(data));
            ++data;
        }
        __asm__ __volatile__ ("emms\n\t" : :);
        if(img.depth() <= 8)
            img.setColorTable(cTable);
    }
    else
#endif
    {
        img.invertPixels(mode);
    }
    return(true);
}

QImage& Blitz::contrast(QImage &img, bool sharpen, int weight)
{
    if(img.isNull())
        return(img);

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QVector<QRgb> cTable;
    if(img.depth() == 8)
        cTable = img.colorTable();

    unsigned int *end, *data;
    int count;
    if(img.depth() > 8){
        count = img.width()*img.height();
        data = (unsigned int *)img.scanLine(0);
    }
    else{
        count = img.numColors();
        data = (unsigned int *)cTable.data();
    }
    end = data+count;

    InlineHSV hsv;
    int v;
    if(sharpen){
        while(data != end){
            hsv.convertRGB2HSV(*data);
            v = hsv.value();
            if(v > 127){
                v += contrast_table[v-128]+weight;
                if(v > 255) v = 255;
            }
            else{
                v -= contrast_table[v]+weight;
                if(v < 0) v = 0;
            }
            hsv.setValue(v);
            hsv.convertHSV2RGB();
            *data = qRgba(hsv.red(), hsv.green(), hsv.blue(), qAlpha(*data));
            ++data;
        }
    }
    else{
        while(data != end){
            hsv.convertRGB2HSV(*data);
            v = hsv.value();
            if(v > 127){
                v -= contrast_table[v-128]+weight;
                if(v < 0) v = 0;
            }
            else{
                v += contrast_table[v]+weight;
                if(v > 255) v = 255;
            }
            hsv.setValue(v);
            hsv.convertHSV2RGB();
            *data = qRgba(hsv.red(), hsv.green(), hsv.blue(), qAlpha(*data));
            ++data;
        }
    }

    if(img.depth() == 8)
        img.setColorTable(cTable);
    return(img);
}

QImage& Blitz::intensity(QImage &img, float percent)
{
    if(img.isNull())
        return(img);

    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QVector<QRgb> colorTable;
    int segmentColors, pixels;
    unsigned int *data;

    if(img.format() == QImage::Format_Indexed8){
        segmentColors = pixels = img.numColors();
        colorTable = img.colorTable();
        data = colorTable.data();
    }
    else{
        segmentColors = 256;
        pixels = img.width()*img.height();
        data = (unsigned int *)img.scanLine(0);
    }

    percent = qBound(-1.0f, percent, 1.0f);
    bool brighten = (percent >= 0);
    if(percent < 0)
        percent = -percent;

#ifdef USE_MMX_INLINE_ASM
#ifdef __GNUC__
#warning Using MMX intensity
#endif
    if(BlitzCPUInfo::haveExtension(BlitzCPUInfo::IntegerSSE)){
        unsigned int rem = pixels % 4;
        pixels -= rem;
        quint32 *end = ( data + pixels );

        quint16 p = (quint16)(256.0f*(percent));
        MMX::Pack4 mult = {{p,p,p,0}};

        __asm__ __volatile__(
        "pxor %%mm7, %%mm7\n\t"                // zero mm7 for unpacking
        "movq  (%0), %%mm6\n\t"                // copy intensity change to mm6
        : : "r"(&mult), "m"(mult));
        if (brighten)
        {
            while ( data != end ) {
                __asm__ __volatile__(
                "movq       (%0), %%mm0\n\t"
                "movq      8(%0), %%mm4\n\t"   // copy 4 pixels of data to mm0 and mm4
                "movq      %%mm0, %%mm1\n\t"
                "movq      %%mm0, %%mm3\n\t"
                "movq      %%mm4, %%mm5\n\t"   // copy to registers for unpacking
                "punpcklbw %%mm7, %%mm0\n\t"
                "punpckhbw %%mm7, %%mm1\n\t"   // unpack the two pixels from mm0
                "pmullw    %%mm6, %%mm0\n\t"
                "punpcklbw %%mm7, %%mm4\n\t"
                "pmullw    %%mm6, %%mm1\n\t"   // multiply by intensity*256
                "psrlw        $8, %%mm0\n\t"   // divide by 256
                "pmullw    %%mm6, %%mm4\n\t"
                "psrlw        $8, %%mm1\n\t"
                "psrlw        $8, %%mm4\n\t"
                "packuswb  %%mm1, %%mm0\n\t"   // pack solution into mm0. saturates at 255
                "movq      %%mm5, %%mm1\n\t"

                "punpckhbw %%mm7, %%mm1\n\t"   // unpack 4th pixel in mm1

                "pmullw    %%mm6, %%mm1\n\t"
                "paddusb   %%mm3, %%mm0\n\t"   // add intesity result to original of mm0
                "psrlw        $8, %%mm1\n\t"
                "packuswb  %%mm1, %%mm4\n\t"   // pack upper two pixels into mm4

                "movq      %%mm0, (%0)\n\t"    // rewrite to memory lower two pixels
                "paddusb   %%mm5, %%mm4\n\t"
                "movq      %%mm4, 8(%0)\n\t"   // rewrite upper two pixels
                : : "r"(data) );
                data += 4;
            }

            end += rem;
            while ( data != end ) {
                __asm__ __volatile__(
                "movd       (%0), %%mm0\n\t"   // repeat above but for
                "punpcklbw %%mm7, %%mm0\n\t"   // one pixel at a time
                "movq      %%mm0, %%mm3\n\t"
                "pmullw    %%mm6, %%mm0\n\t"
                "psrlw        $8, %%mm0\n\t"
                "paddw     %%mm3, %%mm0\n\t"
                "packuswb  %%mm0, %%mm0\n\t"
                "movd      %%mm0, (%0)\n\t"
                : : "r"(data) );
                data++;
            }
        }
        else
        {
            while ( data != end ) {
                __asm__ __volatile__(
                "movq       (%0), %%mm0\n\t"
                "movq      8(%0), %%mm4\n\t"
                "movq      %%mm0, %%mm1\n\t"
                "movq      %%mm0, %%mm3\n\t"

                "movq      %%mm4, %%mm5\n\t"

                "punpcklbw %%mm7, %%mm0\n\t"
                "punpckhbw %%mm7, %%mm1\n\t"
                "pmullw    %%mm6, %%mm0\n\t"
                "punpcklbw %%mm7, %%mm4\n\t"
                "pmullw    %%mm6, %%mm1\n\t"
                "psrlw        $8, %%mm0\n\t"
                "pmullw    %%mm6, %%mm4\n\t"
                "psrlw        $8, %%mm1\n\t"
                "psrlw        $8, %%mm4\n\t"
                "packuswb  %%mm1, %%mm0\n\t"
                "movq      %%mm5, %%mm1\n\t"

                "punpckhbw %%mm7, %%mm1\n\t"

                "pmullw    %%mm6, %%mm1\n\t"
                "psubusb   %%mm0, %%mm3\n\t"   // subtract darkening amount
                "psrlw        $8, %%mm1\n\t"
                "packuswb  %%mm1, %%mm4\n\t"

                "movq      %%mm3, (%0)\n\t"
                "psubusb   %%mm4, %%mm5\n\t"   // only change for this version is
                "movq      %%mm5, 8(%0)\n\t"   // subtraction here as we are darkening image
                : : "r"(data) );
                data += 4;
            }

            end += rem;
            while ( data != end ) {
                __asm__ __volatile__(
                "movd       (%0), %%mm0\n\t"
                "punpcklbw %%mm7, %%mm0\n\t"
                "movq      %%mm0, %%mm3\n\t"
                "pmullw    %%mm6, %%mm0\n\t"
                "psrlw        $8, %%mm0\n\t"
                "psubusw   %%mm0, %%mm3\n\t"
                "packuswb  %%mm3, %%mm3\n\t"
                "movd      %%mm3, (%0)\n\t"
                : : "r"(data) );
                data++;
            }
        }
        __asm__ __volatile__("emms");          // clear mmx state
    }
    else
#endif // USE_MMX_INLINE_ASM
    {
        unsigned char *segmentTable = new unsigned char[segmentColors];
        if(brighten){
            for(int i=0; i < segmentColors; ++i)
                segmentTable[i] = qMin((int)(i*percent), 255);

            int r, g, b;
            for(int i=0; i < pixels; ++i){
                r = qRed(data[i]);
                g = qGreen(data[i]);
                b = qBlue(data[i]);
                data[i] = qRgba(qMin(255, r+segmentTable[r]),
                                qMin(255, g+segmentTable[g]),
                                qMin(255, b+segmentTable[b]), qAlpha(data[i]));
            }
        }
        else{
            for(int i=0; i < segmentColors; ++i)
                segmentTable[i] = qMax((int)(i*percent), 0);

            int r, g, b;
            for(int i=0; i < pixels; ++i){
                r = qRed(data[i]);
                g = qGreen(data[i]);
                b = qBlue(data[i]);
                data[i] = qRgba(qMax(0, r-segmentTable[r]),
                                qMax(0, g-segmentTable[g]),
                                qMax(0, b-segmentTable[b]), qAlpha(data[i]));
            }
        }
        delete[] segmentTable;
    }

    if(img.depth() == 8)
        img.setColorTable(colorTable);
    return(img);
}

QImage& Blitz::channelIntensity(QImage &img, float percent, RGBChannel channel)
{
    if(img.isNull() || (channel != Red && channel != Blue &&
                        channel != Green))
        return(img);
    if(img.format() == QImage::Format_ARGB32_Premultiplied)
        img = img.convertToFormat(QImage::Format_ARGB32);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    QVector<QRgb> colorTable;
    int segmentColors, pixels;
    unsigned int *data;

    if(img.format() == QImage::Format_Indexed8){
        segmentColors = pixels = img.numColors();
        colorTable = img.colorTable();
        data = colorTable.data();
    }
    else{
        segmentColors = 256;
        pixels = img.width()*img.height();
        data = (unsigned int *)img.scanLine(0);
    }

    percent = qBound(-1.0f, percent, 1.0f);
    bool brighten = (percent >= 0);
    if(percent < 0)
        percent = -percent;

    unsigned char *segmentTable = new unsigned char[segmentColors];
    if(brighten){
        for(int i=0; i < segmentColors; ++i)
            segmentTable[i] = qMin((int)(i*percent), 255);

        int color;
        if(channel == Red){ // and here ;-)
            for(int i=0; i < pixels; ++i){
                color = qRed(data[i]);
                data[i] = qRgba(qMin(255, color+segmentTable[color]),
                                qGreen(data[i]), qBlue(data[i]),
                                qAlpha(data[i]));
            }
        }
        else if(channel == Green){
            for(int i=0; i < pixels; ++i){
                color = qGreen(data[i]);
                data[i] = qRgba(qRed(data[i]),
                                qMin(255, color+segmentTable[color]),
                                qBlue(data[i]), qAlpha(data[i]));
            }
        }
        else{
            for(int i=0; i < pixels; ++i){
                color = qBlue(data[i]);
                data[i] = qRgba(qRed(data[i]), qGreen(data[i]),
                                qMin(255, color+segmentTable[color]),
                                qAlpha(data[i]));
            }
        }
    }
    else{
        for(int i=0; i < segmentColors; ++i)
            segmentTable[i] = qMax((int)(i*percent), 0);

        int color;
        if(channel == Red){
            for(int i=0; i < pixels; ++i){
                color = qRed(data[i]);
                data[i] = qRgba(qMax(0, color-segmentTable[color]),
                                qGreen(data[i]), qBlue(data[i]),
                                qAlpha(data[i]));
            }
        }
        else if(channel == Green){
            for(int i=0; i < pixels; ++i){
                color = qGreen(data[i]);
                data[i] = qRgba(qRed(data[i]),
                                qMax(0, color-segmentTable[color]),
                                qBlue(data[i]), qAlpha(data[i]));
            }
        }
        else{
            for(int i=0; i < pixels; ++i){
                color = qBlue(data[i]);
                data[i] = qRgba(qRed(data[i]), qGreen(data[i]),
                                qMax(0, color-segmentTable[color]),
                                qAlpha(data[i]));
            }
        }
    }
    delete[] segmentTable;
    if(img.format() == QImage::Format_Indexed8)
        img.setColorTable(colorTable);
    return(img);
}

QImage& Blitz::desaturate(QImage &img, float desat)
{
    if(img.isNull())
        return(img);
    if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    desat = qBound(0.0f, desat, 1.0f);

    unsigned int *data, *end;
    InlineHSV hsv;
    if(img.format() == QImage::Format_ARGB32 ||
       img.format() == QImage::Format_RGB32 ||
       img.format() == QImage::Format_Indexed8){
        QVector<QRgb> cTable;
        if(img.format() == QImage::Format_Indexed8){
            cTable = img.colorTable();
            data = (unsigned int *)cTable.data();
            end = data + img.numColors();

        }
        else{
            data = (unsigned int *)img.scanLine(0);
            end = data + (img.width()*img.height());
        }
        while(data != end){
            hsv.convertRGB2HSV(*data);
            hsv.setSaturation((int)(hsv.saturation() * (1.0 - desat)));
            hsv.convertHSV2RGB();
            *data = qRgba(hsv.red(), hsv.green(), hsv.blue(), qAlpha(*data));
            ++data;
        }
        if(img.format() == QImage::Format_Indexed8)
            img.setColorTable(cTable);

    }
    else if(img.format() == QImage::Format_ARGB32_Premultiplied){
        data = (unsigned int *)img.scanLine(0);
        end = data + (img.width()*img.height());
        while(data != end){
            hsv.convertRGB2HSV(BlitzPrivate::convertFromPremult(*data));
            hsv.setSaturation((int)(hsv.saturation() * (1.0 - desat)));
            hsv.convertHSV2RGB();
            *data = BlitzPrivate::convertToPremult(qRgba(hsv.red(), hsv.green(),
                                                         hsv.blue(), qAlpha(*data)));
            ++data;
        }
    }
    return(img);
}


QImage Blitz::threshold(QImage &img, unsigned char thresholdValue,
                        RGBChannel channel, unsigned int c_on,
                        unsigned int c_off)
{
    if(img.isNull())
        return(img);
    else if(img.depth() < 8)
        img = img.convertToFormat(QImage::Format_Indexed8);

    int x, y, w, h;
    w = img.width(); h = img.height();
    QImage buffer(img.width(), img.height(), QImage::Format_Indexed8);

    QVector<QRgb> cTable(2);
    cTable[0] = c_off; cTable[1] = c_on;
    buffer.setColorTable(cTable);

    unsigned char *dest;
    if(img.format() == QImage::Format_RGB32 ||
       img.format() == QImage::Format_ARGB32){
        QRgb *src = (QRgb *)img.scanLine(0);
        switch(channel){
        case Grayscale:
        case All:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (qGray(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Brightness:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (BlitzPrivate::brightness(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Red:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (qRed(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Green:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (qGreen(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Blue:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (qBlue(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Alpha:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x)
                    *dest++ = (qAlpha(*src++) >= thresholdValue) ? 1 : 0;
            }
            break;
        }
        }
    }
    else if(img.format() == QImage::Format_ARGB32_Premultiplied){
        QRgb *src = (QRgb *)img.scanLine(0);
        switch(channel){
        case Grayscale:
        case All:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (qGray(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Brightness:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (BlitzPrivate::brightness(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Red:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (qRed(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Green:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (qGreen(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Blue:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (qBlue(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        case Alpha:{
            for(y=0; y < h; ++y){
                dest = buffer.scanLine(y);
                for(x=0; x < w; ++x, ++src)
                    *dest++ = (qAlpha(BlitzPrivate::convertFromPremult(*src)) >=
                               thresholdValue) ? 1 : 0;
            }
            break;
        }
        }
    }
    else{
        // would be quicker to just threshold the color table, but we return
        // images w/ only 2 colors for easy conversion to bitmap
        QVector<QRgb> srcTable(img.colorTable());
        unsigned char *src;
        switch(channel){
        case Grayscale:
        case All:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (qGray(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        case Brightness:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (BlitzPrivate::brightness(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        case Red:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (qRed(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        case Green:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (qGreen(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        case Blue:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (qBlue(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        case Alpha:{
            for(y=0; y < h; ++y){
                src = img.scanLine(y); dest = buffer.scanLine(y);
                for(x=0; x < w; ++x){
                    *dest++ = (qAlpha(srcTable[*src++]) >= thresholdValue) ?
                        1 : 0;
                }
            }
            break;
        }
        }
    }
    return(buffer);

}

QImage& Blitz::fade(QImage &img, float val, const QColor &color)
{
    if(img.isNull() || img.depth() == 1)
        return(img);

    val = qBound(0.0f, val, 1.0f);
    unsigned char tbl[256];
    for(int i=0; i < 256; ++i)
        tbl[i] = (int)(val * i + 0.5);

    int red = color.red();
    int green = color.green();
    int blue = color.blue();

    int r, g, b;
    QRgb *data, *end;
    QVector<QRgb> cTable;

    if(img.format() == QImage::Format_Indexed8){
        cTable = img.colorTable();
        data = (unsigned int *)cTable.data();
        end = data + img.numColors();

    }
    else{
        data = (unsigned int *)img.scanLine(0);
        end = data + (img.width()*img.height());
    }

    if(img.format() == QImage::Format_ARGB32_Premultiplied){
        QRgb col;
        while(data != end){
            col = BlitzPrivate::convertFromPremult(*data);
            r = qRed(col); g = qGreen(col); b = qBlue(col);
            *data++ =
                BlitzPrivate::convertToPremult(qRgba((r > red) ? r - tbl[r - red] : r + tbl[red - r],
                                       (g > green) ? g - tbl[g - green] : g + tbl[green - g],
                                       (b > blue)  ? b - tbl[b - blue] : b + tbl[blue - b],
                                       qAlpha(col)));
        }
    }
    else{
        while(data != end){
            r = qRed(*data); g = qGreen(*data); b = qBlue(*data);
            *data = qRgba((r > red) ? r - tbl[r - red] : r + tbl[red - r],
                          (g > green) ? g - tbl[g - green] : g + tbl[green - g],
                          (b > blue)  ? b - tbl[b - blue] : b + tbl[blue - b],
                          qAlpha(*data));
            ++data;
        }
    }
    if(img.format() == QImage::Format_Indexed8)
        img.setColorTable(cTable);
    return(img);
}

QImage& Blitz::flatten(QImage &img, const QColor &ca, const QColor &cb)
{
    if(img.isNull())
        return(img);

    if(img.depth() == 1) {
        img.setColor(0, ca.rgb());
        img.setColor(1, cb.rgb());
        return(img);
    }

    int r1 = ca.red(); int r2 = cb.red();
    int g1 = ca.green(); int g2 = cb.green();
    int b1 = ca.blue(); int b2 = cb.blue();
    int min = 0, max = 255;

    QRgb *data, *end;
    QVector<QRgb> cTable;
    if(img.format() == QImage::Format_Indexed8){
        cTable = img.colorTable();
        data = (unsigned int *)cTable.data();
        end = data + img.numColors();

    }
    else{
        data = (unsigned int *)img.scanLine(0);
        end = data + (img.width()*img.height());
    }

    // get minimum and maximum graylevel
    QRgb *ptr = data;
    int mean;

    if(img.format() != QImage::Format_ARGB32_Premultiplied){
        while(ptr != end){
            mean = (qRed(*ptr) + qGreen(*ptr) + qBlue(*ptr)) / 3;
            min = qMin(min, mean);
            max = qMax(max, mean);
            ++ptr;
        }
    }
    else{
        QRgb pixel;
        while(ptr != end){
            pixel = BlitzPrivate::convertFromPremult(*ptr);
            mean = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
            min = qMin(min, mean);
            max = qMax(max, mean);
            ++ptr;
        }
    }

    // conversion factors
    float sr = ((float) r2 - r1) / (max - min);
    float sg = ((float) g2 - g1) / (max - min);
    float sb = ((float) b2 - b1) / (max - min);

    if(img.format() != QImage::Format_ARGB32_Premultiplied){
        while(data != end){
            mean = (qRed(*data) + qGreen(*data) + qBlue(*data)) / 3;
            *data = qRgba((unsigned char)(sr * (mean - min) + r1 + 0.5),
                          (unsigned char)(sg * (mean - min) + g1 + 0.5),
                          (unsigned char)(sb * (mean - min) + b1 + 0.5),
                          qAlpha(*data));
            ++data;
        }
    }
    else{
        QRgb pixel;
        while(data != end){
            pixel = BlitzPrivate::convertFromPremult(*data);
            mean = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
            *data =
                BlitzPrivate::
                convertToPremult(qRgba((unsigned char)(sr * (mean - min) + r1 + 0.5),
                                       (unsigned char)(sg * (mean - min) + g1 + 0.5),
                                       (unsigned char)(sb * (mean - min) + b1 + 0.5),
                                       qAlpha(*data)));
            ++data;
        }
    }

    if(img.format() == QImage::Format_Indexed8)
        img.setColorTable(cTable);
    return(img);
}


