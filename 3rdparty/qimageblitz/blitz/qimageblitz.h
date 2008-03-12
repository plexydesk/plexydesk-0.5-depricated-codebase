#ifndef __BLITZ_H
#define __BLITZ_H

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

#include <QtGui/QImage>
#include "qimageblitz_export.h"

class QIMAGEBLITZ_EXPORT Blitz
{
public:
    enum GradientType {VerticalGradient=0, HorizontalGradient, DiagonalGradient,
        CrossDiagonalGradient, PyramidGradient, RectangleGradient,
        PipeCrossGradient, EllipticGradient};

    enum RGBChannel{Grayscale=0, Brightness, Red, Green, Blue, Alpha, All};
    enum EffectQuality{Low=0, High};

    enum ScaleFilterType{UndefinedFilter=0, PointFilter, BoxFilter,
        TriangleFilter, HermiteFilter, HanningFilter, HammingFilter,
        BlackmanFilter, GaussianFilter, QuadraticFilter, CubicFilter,
        CatromFilter, MitchellFilter, LanczosFilter, BesselFilter,
        SincFilter};

    enum ModulationType{Intensity=0, Saturation, HueShift, Contrast};

    /**
     * Inverts an image. If the machine supports MMX this can do two pixels
     * at a time, otherwise it's the same as QImage::invertPixels().
     *
     * @param img The image to invert.
     * @param mode If to invert the alpha channel or not.
     * @author Daniel M. Duley (mosfet)
     */
    static bool invert(QImage &img, QImage::InvertMode mode =
                       QImage::InvertRgb);
    /**
     * Grayscales an image.
     *
     * @param img The image to grayscale.
     * @param reduceDepth If true the result will be an 8bit palette image.
     * @author Daniel M. Duley (mosfet)
     */
    static bool grayscale(QImage &img, bool reduceDepth=false);
    /**
     * A fast smoothscale method with MMX support.
     *
     * @param img The image to smoothscale.
     * @param sz The size to scale to.
     * @param aspectRatio What aspect ratio to use, if any.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage smoothScale(QImage &img, const QSize &sz,
                              Qt::AspectRatioMode aspectRatio =
                              Qt::IgnoreAspectRatio);
    static QImage smoothScale(QImage &img, int dw, int dh,
                              Qt::AspectRatioMode aspectRatio =
                              Qt::IgnoreAspectRatio);
    /**
     * Smoothscales an image using a high-quality filter.
     *
     * @param img The image to smoothscale.
     * @param sz The size to scale to.
     * @param blur A blur factor. Values greater than 1.0 blur while values
     * less than 1.0 sharpen.
     * @param filter The filter type.
     * @param aspectRatio What aspect ratio to use, if any.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage smoothScaleFilter(QImage &img, const QSize &sz,
                                    float blur=1.0,
                                    ScaleFilterType filter=BlackmanFilter,
                                    Qt::AspectRatioMode aspectRatio =
                                    Qt::IgnoreAspectRatio);
    static QImage smoothScaleFilter(QImage &img, int dwX, int dwY,
                                    float blur=1.0,
                                    ScaleFilterType filter=BlackmanFilter,
                                    Qt::AspectRatioMode aspectRatio =
                                    Qt::IgnoreAspectRatio);

    /**
     * Convolves an image using a convolution matrix, (kernel), and provides
     * the basis for many different image filters and effects.
     *
     * @param img The image to process.
     * @param matrix_size The size of one side of the convolution matrix, ie:
     * a 3x3 matrix would be 3, not 9.
     * @param matrix The convolution matrix.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage convolve(QImage &img, int matrix_size, float *matrix);
    /**
     * Same as above but with a faster integer matrix.
     */
    static QImage convolveInteger(QImage &img, int matrix_size, int *matrix,
                                  int divisor=0);
    /**
     * A fast blur algorithm.
     *
     * @param img The image to process.
     * @param radius The radius of the blur. Larger values blur more.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage blur(QImage &img, int radius=3);
    /**
     * A high quality gaussian/convolve-based blur.
     *
     * @param img The image to process.
     * @param radius The radius of the gaussian, not counting the center
     * pixel. Use 0, (recommended), and an appropriate one will be used.
     * @param sigma The standard deviation of the gaussian in pixels. Use
     * 1.0, (recommended), for the default value.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage gaussianBlur(QImage &img, float radius=0.0, float sigma=1.0);
    /**
     * Detects edges in an image using the Sobel algorithm.
     *
     * @param img The image to process.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage edge(QImage &img);
    /**
     * Detects edges in an image using a convolution matrix.
     *
     * @param img The image to process.
     * @param radius The radius of the gaussian, not counting the center
     * pixel. Use 0, (recommended), and an appropriate one will be used.
     * @param quality Determines if to use a small or large convolution matrix.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage convolveEdge(QImage &img, float radius=0.0,
                               EffectQuality quality=High);
    /**
     * Sharpens an image.
     *
     * @param img The image to process.
     * @param radius The radius of the gaussian, not counting the center
     * pixel. Use 0, (recommended), and an appropriate one will be used.
     * @param sigma The standard deviation of the gaussian in pixels. Use
     * 1.0, (recommended), for the default value.
     * @param quality Determines if to use a small or large convolution matrix.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage gaussianSharpen(QImage &img, float radius=0.0, float sigma=1.0,
                                  EffectQuality quality=High);
    /**
     * A cheap, fast sharpen.
     *
     * @param radius The radius. Larger values sharpen more.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage sharpen(QImage &img, int radius=3);
    /**
     * Embosses an image.
     *
     * @param img The image to process.
     * @param radius The radius of the gaussian, not counting the center
     * pixel. Use 0, (recommended), and an appropriate one will be used.
     * @param sigma The standard deviation of the gaussian in pixels. Use
     * 1.0, (recommended), for the default value.
     * @param quality Determines if to use a small or large convolution matrix.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage emboss(QImage &img, float radius=0.0, float sigma=1.0,
                         EffectQuality quality=High);
    /**
     * Antialiases an image.
     *
     * @param img The image to process.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage antialias(QImage &img);
    /**
     * Minimizes speckle noise in the source image using the 8 hull
     * algorithm.
     *
     * @param img The image to process.
     * @return A reference to the image for convenience.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage& despeckle(QImage &img);
    /**
     * High quality, fast HSV contrast.
     *
     * @param img The image to process.
     * @param sharpen If true sharpness is increase, (spiffed). Otherwise
     * it is decreased, (dulled).
     * @param weight How much to spiff or dull.
     * @return A reference to the image for convenience.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage& contrast(QImage &img, bool sharpen, int weight=3);
    /**
     * Performs histogram equalization.
     *
     * @param img The image to equalize.
     * @author Daniel M. Duley (mosfet)
     */
    static bool equalize(QImage &img);
    /**
     * Normalizes the pixel values to span the full range of color values.
     * This is a contrast enhancement technique.
     *
     * @param img The image to normalize
     * @author Daniel M. Duley (mosfet)
     */
    static bool normalize(QImage &img);
    /**
     * Either brighten or dim the image by a specified percent. For example,
     * 0.50 will modify the colors by 50%.
     *
     * @param img The image to process.
     * @param percent The percent value. Use a negative value to dim.
     * @return A reference to the image for convenience.
     * @author Daniel M. Duley (mosfet)
     * @author Benjamin Roe (ben@benroe.com)
     */
    static QImage& intensity(QImage &img, float percent);
    /**
     * Modifies the intensity of an image's RGB channel.
     *
     * @param img The image to process.
     * @param percent Percent value. Use a negative value to dim.
     * @param channel Which channel(s) should be modified. Only Red, Green,
     * and Blue are valid.
     * @return A reference to the image for convenience.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage& channelIntensity(QImage &img, float percent,
                                    RGBChannel channel);
    /**
     * Desaturate an image evenly.
     *
     * @param img The image to process.
     * @param desat A value between 0 and 1 setting the degree of desaturation
     * @return A reference to the image for convenience.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage& desaturate(QImage &img, float desat = 0.5);
    /**
     * Fade an image to a certain background color. The number of colors will
     * not be changed.
     *
     * @param img   The image to process.
     * @param val   The strength of the effect. 0 <= val <= 1.
     * @param color The background color.
     * @return      A reference to the image for convenience.
     */
    static QImage& fade(QImage &img, float val, const QColor &color);

    /**
     * This recolors a pixmap. The most dark color will become color a,
     * the most bright one color b, and in between.
     *
     * @param img The image to process.
     * @param ca Color a
     * @param cb Color b
     */
    static QImage& flatten(QImage &img, const QColor &ca, const QColor &cb);
    /**
     * Thresholds an image based on a given channel and threshold value.
     *
     * @param img The image to process.
     * @param thresholdValue The value that separates "on" colors from "off"
     * ones.
     * @param channel The channel to use when thresholding.
     * @param aboveColor The color to use for values at or above the threshold.
     * @param belowColor The color to use for values below the threshold.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage threshold(QImage &img, unsigned char thresholdValue=127,
                            RGBChannel channel=Grayscale,
                            unsigned int aboveColor=qRgb(255, 255, 255),
                            unsigned int belowColor=qRgb(0, 0, 0));
    /**
     * Swirls the image by a specified amount
     *
     * @param img The image to process.
     * @param degrees The tightness of the swirl.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage swirl(QImage &img, float degrees=60.0);
    /**
     * Implodes an image by a specified percent.
     *
     * @param img The QImage to process.
     * @param amount The extent of the implosion.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage implode(QImage &img, float amount=0.3);
    /**
     * Modifies the pixels along a sine wave.
     *
     * @param img        The image to process.
     * @param amplitude  The amplitude of the sine wave.
     * @param frequency  The frequency of the sine wave.
     * @param background An RGBA value to use for the background. After the
     * effect some pixels may be "empty". This value is used for those pixels.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage wave(QImage &img, float amplitude=25.0, float frequency=150.0,
                       unsigned int background=0);
    /**
     * Produces an oil painting effect.
     *
     * @param img The image to process.
     * @param radius The radius of the gaussian, not counting the center
     * pixel. Use 0, (recommended), and an appropriate one will be used.
     * @param quality Determines if to use a small or large convolution matrix.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage oilPaint(QImage &img, float radius=0.0,
                           EffectQuality quality=High);
    /**
     * Produces a "charcoal" image effect.
     *
     * @param img The image to process.
     * @return The processed image. The original is not changed.
     * @author Daniel M. Duley (mosfet)
     */
    static QImage charcoal(QImage &img);
    /**
     * Create a gradient from color a to color b of the specified type.
     *
     * @param size The desired size of the gradient.
     * @param ca Color a
     * @param cb Color b
     * @param type The type of gradient.
     * @return The gradient.
     */
    static QImage gradient(const QSize &size, const QColor &ca,
                           const QColor &cb, GradientType type);
    /**
     * Creates an 8bit grayscale gradient suitable for use as an alpha channel
     * using QImage::setAlphaChannel().
     *
     * @param size The desired size of the gradient.
     * @param ca The grayscale start value.
     * @param cb The grayscale end value.
     * @param type The type of gradient.
     * @return The gradient.
     */
    static QImage grayGradient(const QSize &size, unsigned char ca,
                               unsigned char cb, GradientType type);
    /**
     * Create an unbalanced gradient. An unbalanced gradient is a gradient
     * where the transition from color a to color b is not linear, but in this
     * case exponential.
     *
     * @param size The desired size of the gradient.
     * @param ca Color a
     * @param cb Color b
     * @param type The type of gradient.
     * @param xfactor The x decay length. Use a value between -200 and 200.
     * @param yfactor The y decay length.
     * @return The gradient.
     */
    static QImage unbalancedGradient(const QSize &size, const QColor &ca,
                                     const QColor &cb, GradientType type,
                                     int xfactor=100, int yfactor=100);
    /**
     * Creates an 8bit grayscale gradient suitable for use as an alpha channel
     * using QImage::setAlphaChannel().
     *
     * @param size The desired size of the gradient.
     * @param type The type of gradient.
     * @param ca The grayscale start value.
     * @param cb The grayscale end value.
     * @param xfactor The x decay length. Use a value between -200 and 200.
     * @param yfactor The y decay length.
     * @return The gradient.
     */
    static QImage grayUnbalancedGradient(const QSize &size, unsigned char ca,
                                         unsigned char cb, GradientType type,
                                         int xfactor=100, int yfactor=100);
    /**
     * Modulate the image with a color channel of another image.
     *
     * @param img The QImage to modulate and result.
     * @param modImg The QImage to use for modulation.
     * @param reverse Invert the meaning of image/modImage; result is image!
     * @param type The modulation Type to use.
     * @param factor The modulation amplitude; with 0 no effect [-200;200].
     * @param channel The RBG channel of image2 to use for modulation.
     * @return Returns the image(), provided for convenience.
     */
    static QImage& modulate(QImage &img, QImage &modImg, bool reverse,
                            ModulationType type, int factor,
                            RGBChannel channel);
};

#endif

