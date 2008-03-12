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
#include <QString>
#include <QVector>

// These are used as accumulators

typedef struct
{
    qint32 red, green, blue, alpha;
} IntegerPixel;

typedef struct
{
    qint16 red, green, blue, alpha;
} ShortPixel;

typedef struct
{
    // Yes, a normal pixel can be used instead but this is easier to read
    // and no shifts to get components.
    qint8 red, green, blue, alpha;
} CharPixel;

typedef struct
{
    quint32 red, green, blue, alpha;
} HistogramListItem;


bool Blitz::equalize(QImage &img)
{
    if(img.isNull())
        return(false);

    HistogramListItem *histogram;
    IntegerPixel *map;
    IntegerPixel intensity, high, low;
    CharPixel *equalize_map;
    int i, count;
    QRgb pixel, *dest;
    unsigned char r, g, b;

    if(img.depth() < 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    count = img.width()*img.height();

    map = new IntegerPixel[256];
    histogram = new HistogramListItem[256];
    equalize_map = new CharPixel[256];

    // form histogram
    memset(histogram, 0, 256*sizeof(HistogramListItem));
    dest = (QRgb *)img.bits();

    if(img.format() == QImage::Format_ARGB32_Premultiplied){
        for(i=0; i < count; ++i, ++dest){
            pixel = BlitzPrivate::convertFromPremult(*dest);
            histogram[qRed(pixel)].red++;
            histogram[qGreen(pixel)].green++;
            histogram[qBlue(pixel)].blue++;
            histogram[qAlpha(pixel)].alpha++;
        }
    }
    else{
        for(i=0; i < count; ++i){
            pixel = *dest++;
            histogram[qRed(pixel)].red++;
            histogram[qGreen(pixel)].green++;
            histogram[qBlue(pixel)].blue++;
            histogram[qAlpha(pixel)].alpha++;
        }
    }

    // integrate the histogram to get the equalization map
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(i=0; i < 256; ++i){
        intensity.red += histogram[i].red;
        intensity.green += histogram[i].green;
        intensity.blue += histogram[i].blue;
        map[i] = intensity;
    }

    low = map[0];
    high = map[255];
    memset(equalize_map, 0, 256*sizeof(CharPixel));
    for(i=0; i < 256; ++i){
        if(high.red != low.red)
            equalize_map[i].red=(unsigned char)
                ((255*(map[i].red-low.red))/(high.red-low.red));
        if(high.green != low.green)
            equalize_map[i].green=(unsigned char)
                ((255*(map[i].green-low.green))/(high.green-low.green));
        if(high.blue != low.blue)
            equalize_map[i].blue=(unsigned char)
                ((255*(map[i].blue-low.blue))/(high.blue-low.blue));
    }

    // stretch the histogram and write
    dest = (QRgb *)img.bits();
    if(img.format() == QImage::Format_ARGB32_Premultiplied){
        for(i=0; i < count; ++i, ++dest){
            pixel = BlitzPrivate::convertFromPremult(*dest);
            r = (low.red != high.red) ? equalize_map[qRed(pixel)].red :
                qRed(pixel);
            g = (low.green != high.green) ? equalize_map[qGreen(pixel)].green :
                qGreen(pixel);
            b = (low.blue != high.blue) ?  equalize_map[qBlue(pixel)].blue :
                qBlue(pixel);
            *dest = BlitzPrivate::convertToPremult(qRgba(r, g, b, qAlpha(pixel)));
        }
    }
    else{
        for(i=0; i < count; ++i){
            pixel = *dest;
            r = (low.red != high.red) ? equalize_map[qRed(pixel)].red :
                qRed(pixel);
            g = (low.green != high.green) ? equalize_map[qGreen(pixel)].green :
                qGreen(pixel);
            b = (low.blue != high.blue) ?  equalize_map[qBlue(pixel)].blue :
                qBlue(pixel);
            *dest++ = qRgba(r, g, b, qAlpha(pixel));
        }
    }

    delete[] histogram;
    delete[] map;
    delete[] equalize_map;
    return(true);
}

bool Blitz::normalize(QImage &img)
{
    if(img.isNull())
        return(false);

    IntegerPixel intensity;
    HistogramListItem *histogram;
    CharPixel *normalize_map;
    ShortPixel high, low;

    int threshold_intensity;
    int i, count;
    QRgb pixel, *dest;
    unsigned char r, g, b;

    if(img.depth() < 32){
        img = img.convertToFormat(img.hasAlphaChannel() ?
                                  QImage::Format_ARGB32 :
                                  QImage::Format_RGB32);
    }
    count = img.width()*img.height();

    histogram = new HistogramListItem[256];
    normalize_map = new CharPixel[256];

    // form histogram
    memset(histogram, 0, 256*sizeof(HistogramListItem));
    dest = (QRgb *)img.bits();

    if(img.format() == QImage::Format_ARGB32_Premultiplied){
        for(i=0; i < count; ++i, ++dest){
            pixel = BlitzPrivate::convertFromPremult(*dest);
            histogram[qRed(pixel)].red++;
            histogram[qGreen(pixel)].green++;
            histogram[qBlue(pixel)].blue++;
            histogram[qAlpha(pixel)].alpha++;
        }
    }
    else{
        for(i=0; i < count; ++i){
            pixel = *dest++;
            histogram[qRed(pixel)].red++;
            histogram[qGreen(pixel)].green++;
            histogram[qBlue(pixel)].blue++;
            histogram[qAlpha(pixel)].alpha++;
        }
    }

    // find the histogram boundaries by locating the .01 percent levels.
    threshold_intensity = count/1000;

    memset(&intensity, 0, sizeof(IntegerPixel));
    for(low.red=0; low.red < 256; ++low.red){
        intensity.red += histogram[low.red].red;
        if(intensity.red > threshold_intensity)
            break;
    }
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(high.red=255; high.red >= 0; --high.red){
        intensity.red += histogram[high.red].red;
        if(intensity.red > threshold_intensity)
            break;
    }
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(low.green=low.red; low.green < high.red; ++low.green){
        intensity.green += histogram[low.green].green;
        if(intensity.green > threshold_intensity)
            break;
    }
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(high.green=high.red; high.green != low.red; --high.green){
        intensity.green += histogram[high.green].green;
        if(intensity.green > threshold_intensity)
            break;
    }
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(low.blue=low.green; low.blue < high.green; ++low.blue){
        intensity.blue += histogram[low.blue].blue;
        if(intensity.blue > threshold_intensity)
            break;
    }
    memset(&intensity, 0, sizeof(IntegerPixel));
    for(high.blue=high.green; high.blue != low.green; --high.blue){
        intensity.blue += histogram[high.blue].blue;
        if(intensity.blue > threshold_intensity)
            break;
    }

    delete[] histogram;

    // stretch the histogram to create the normalized image mapping.
    for(i=0; i < 256; i++){
        if(i < low.red)
            normalize_map[i].red = 0;
        else{
            if(i > high.red)
                normalize_map[i].red = 255;
            else if(low.red != high.red)
                normalize_map[i].red = (255*(i-low.red))/
                    (high.red-low.red);
        }

        if(i < low.green)
            normalize_map[i].green = 0;
        else{
            if(i > high.green)
                normalize_map[i].green = 255;
            else if(low.green != high.green)
                normalize_map[i].green = (255*(i-low.green))/
                    (high.green-low.green);
        }

        if(i < low.blue)
            normalize_map[i].blue = 0;
        else{
            if(i > high.blue)
                normalize_map[i].blue = 255;
            else if(low.blue != high.blue)
                normalize_map[i].blue = (255*(i-low.blue))/
                    (high.blue-low.blue);
        }
    }

    // write
    dest = (QRgb *)img.bits();
    if(img.format() == QImage::Format_ARGB32_Premultiplied){
        for(i=0; i < count; ++i, ++dest){
            pixel = BlitzPrivate::convertFromPremult(*dest);
            r = (low.red != high.red) ? normalize_map[qRed(pixel)].red :
                qRed(pixel);
            g = (low.green != high.green) ? normalize_map[qGreen(pixel)].green :
                qGreen(pixel);
            b = (low.blue != high.blue) ?  normalize_map[qBlue(pixel)].blue :
                qBlue(pixel);
            *dest = BlitzPrivate::convertToPremult(qRgba(r, g, b, qAlpha(pixel)));
        }
    }
    else{
        for(i=0; i < count; ++i){
            pixel = *dest;
            r = (low.red != high.red) ? normalize_map[qRed(pixel)].red :
                qRed(pixel);
            g = (low.green != high.green) ? normalize_map[qGreen(pixel)].green :
                qGreen(pixel);
            b = (low.blue != high.blue) ?  normalize_map[qBlue(pixel)].blue :
                qBlue(pixel);
            *dest++ = qRgba(r, g, b, qAlpha(pixel));
        }
    }

    delete[] normalize_map;
    return(true);
}

QImage Blitz::oilPaint(QImage &img, float radius,
                       EffectQuality quality)
{
    int matrix_size = BlitzPrivate::defaultConvolveMatrixSize(radius, 0.5,
                                                              quality == High);
    int i, x, y, w, h, matrix_x, matrix_y;
    int edge = matrix_size/2;
    unsigned int max, value;
    QRgb *dest, *src, *s, **scanblock;

    w = img.width();
    h = img.height();
    if(w < 3 || h < 3){
        qWarning("Blitz::oilPaint(): Image is too small!");
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
    unsigned int *histogram = new unsigned int[256];

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
            (void)memset(histogram, 0, 256*sizeof(unsigned int));
            max = 0;
            for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                s = scanblock[matrix_y];
                matrix_x = -edge;
                while(x+matrix_x < 0){
                    value = qGray(*s);
                    histogram[value]++;
                    if(histogram[value] > max){
                        max = histogram[value];
                        *dest = *s;
                    }
                    ++matrix_x;
                }
                while(matrix_x <= edge){
                    value = qGray(*s);
                    histogram[value]++;
                    if(histogram[value] > max){
                        max = histogram[value];
                        *dest = *s;
                    }
                    ++matrix_x; ++s;
                }
            }
            ++dest;
        }

        // Okay, now process the middle part where the entire neighborhood
        // is on the image.
        for(; x+edge < w; ++x){
            (void)memset(histogram, 0, 256*sizeof(unsigned int));
            max = 0;
            for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                s = scanblock[matrix_y] + (x-edge);
                for(matrix_x = -edge; matrix_x <= edge; ++matrix_x, ++s){
                    value = qGray(*s);
                    histogram[value]++;
                    if(histogram[value] > max){
                        max = histogram[value];
                        *dest = *s;
                    }
                }
            }
            ++dest;
        }

        // Finally process the right part where the neighborhood extends off
        // the right edge of the image
        for(; x < w; ++x){
            (void)memset(histogram, 0, 256*sizeof(unsigned int));
            max = 0;
            for(matrix_y = 0; matrix_y < matrix_size; ++matrix_y){
                s = scanblock[matrix_y];
                s += x-edge;
                matrix_x = -edge;
                while(x+matrix_x < w){
                    value = qGray(*s);
                    histogram[value]++;
                    if(histogram[value] > max){
                        max = histogram[value];
                        *dest = *s;
                    }
                    ++matrix_x, ++s;
                }
                --s;
                while(matrix_x <= edge){
                    value = qGray(*s);
                    histogram[value]++;
                    if(histogram[value] > max){
                        max = histogram[value];
                        *dest = *s;
                    }
                    ++matrix_x;
                }
            }
            ++dest;
        }
    }

    delete[] histogram;
    delete[] scanblock;
    return(buffer);
}

