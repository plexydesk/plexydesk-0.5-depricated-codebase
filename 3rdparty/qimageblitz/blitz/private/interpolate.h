#ifndef __PIXIE_INTERPOLATE_H
#define __PIXIE_INTERPOLATE_H

/*
 Copyright (C) 2005, 2007 Daniel M. Duley <daniel.duley@verizon.net>

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

#include <QImage>
#include <QVector>
#include <cmath>

/**
 * Performs interpolation between a pixel's colors and it's neighbors.
 * Handles 32bpp and 8bpp source images transparently.
 *
 * @short Inline interpolation
 * @author Daniel M. Duley
 */ 
class InlineInterpolate
{
public:
    /**
     * Constructs an interpolation object. Do this outside your main loop then
     * call the inline interpolate methods when you need them.
     *
     * @param image The image whose pixels you will be interpolating. Once
     * you have created an interpolation object you can change the image's
     * pixel data, but do not resize or reset it.
     * @param background The background color to use if a pixel's neighbor
     * is off the edge of the image.
     */
    InlineInterpolate(QImage *image, unsigned int background){
        img = image; ptr = img->bits();
        colorTable = img->colorTable();
        w=img->width(); h=img->height();
        bg=background; truecolor = img->depth() > 8;
    }
    /**
     * Interpolates the pixel at the given x,y position. If the pixel's
     * neighbor is off the edge of the image the last pixel is extended.
     */
    inline unsigned int interpolate(float x_offset, float y_offset);
    /**
     * Interpolates the pixel at the given x,y position. If the pixel's
     * neighbor is off the edge of the image the background color is used.
     */
    inline unsigned int interpolateBackground(float x_offset, float y_offset);
private:
    int w, h;
    unsigned int p, q, r, s, bg;
    unsigned char *ptr;
    QVector<QRgb> colorTable;
    bool truecolor;
    QImage *img;
};

inline unsigned int InlineInterpolate::interpolate(float x_offset,
                                                   float y_offset)
{
    int x = qBound(0, (int)x_offset, w-2);
    int y = qBound(0, (int)y_offset, h-2);

    if(truecolor){
        p = *(((QRgb *)ptr)+(y*w)+x);
        q = *(((QRgb *)ptr)+(y*w)+x+1);
        r = *(((QRgb *)ptr)+((y+1)*w)+x);
        s = *(((QRgb *)ptr)+((y+1)*w)+x+1);
    }
    else{
        p = colorTable[*(ptr+(y*w)+x)];
        q = colorTable[*(ptr+(y*w)+x+1)];
        r = colorTable[*(ptr+((y+1)*w)+x)];
        s = colorTable[*(ptr+((y+1)*w)+x+1)];
    }
    x_offset -= std::floor(x_offset); y_offset -= std::floor(y_offset);
    unsigned int alpha = (unsigned int)(255*x_offset);
    unsigned int beta = (unsigned int)(255*y_offset);

    p = BlitzPrivate::interpolate255(p, 255-alpha, q, alpha);
    r = BlitzPrivate::interpolate255(r, 255-alpha, s, alpha);
    return(BlitzPrivate::interpolate255(p, 255-beta, r, beta));
}

inline unsigned int InlineInterpolate::interpolateBackground(float x_offset,
                                                             float y_offset)
{
    int x = (int)x_offset;
    int y = (int)y_offset;
    p = q = r = s = bg;

    if(truecolor){
        if(y >= 0 && y < h && x >= 0 && x < w){
            p = *(((QRgb *)ptr)+(y*w)+x);
            if(y+1 < h) r = *(((QRgb *)ptr)+((y+1)*w)+x);
            if(x+1 < w){
                q = *(((QRgb *)ptr)+(y*w)+x+1);
                if(y+1 < h) q = *(((QRgb *)ptr)+((y+1)*w)+x+1);
            }
        }
    }
    else{
        if(y >= 0 && y < h && x >= 0 && x < w){
            p = colorTable[*(ptr+(y*w)+x)];
            if(y+1 < h) r = colorTable[*(ptr+((y+1)*w)+x)];
            if(x+1 < w){
                q = colorTable[*(ptr+(y*w)+x+1)];
                if(y+1 < h) s = colorTable[*(ptr+((y+1)*w)+x+1)];
            }
        }
    }
    x_offset -= std::floor(x_offset); y_offset -= std::floor(y_offset);
    unsigned int alpha = (unsigned int)(255*x_offset);
    unsigned int beta = (unsigned int)(255*y_offset);

    p = BlitzPrivate::interpolate255(p, 255-alpha, q, alpha);
    r = BlitzPrivate::interpolate255(r, 255-alpha, s, alpha);
    return(BlitzPrivate::interpolate255(p, 255-beta, r, beta));
}

#endif

