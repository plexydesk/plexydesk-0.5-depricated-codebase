#ifndef __BLITZTEST_MAINWINDOW_H
#define __BLITZTEST_MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QLabel;
class QCheckBox;

class BlitzMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    BlitzMainWindow();
    void openFile(const QString &filename);
private slots:
    void slotOpen();
    void slotRevert();

    void slotGrayscale();
    void slotInvert();
    void slotEqualize();
    void slotNormalize();
    void slotDespeckle();

    void slotSharpen();
    void slotGaussianSharpen();
    void slotSpiff();
    void slotDull();

    void slotDesaturate();
    void slotFade();
    void slotFlatten();

    void slotBlur();
    void slotGaussianBlur();

    void slotEdge();
    void slotConvolveEdge();

    void slotCharcoal();
    void slotSwirl();
    void slotImplode();
    void slotWave();
    void slotOilpaint();
    void slotEmboss();
    void slotAntialias();

    void slotSmoothscale();
    void slotSmoothscaleFiltered();

    void slotModulate();
    void slotGradient();
    void slotUnbalancedGradient();
    void slotGrayscaleGradient();
    void slotGrayscaleUnbalancedGradient();

    void slotThreshold();
    void slotThresholdRed();
    void slotThresholdGreen();
    void slotThresholdBlue();
    void slotThresholdAlpha();

    void slotIncIntensity();
    void slotDecIntensity();
    void slotIncRedIntensity();
    void slotDecRedIntensity();
    void slotIncGreenIntensity();
    void slotDecGreenIntensity();
    void slotIncBlueIntensity();
    void slotDecBlueIntensity();
private:
    QImage img;
    QString fn;
    QLabel *lbl;
    QCheckBox *qualityCB;
};

#endif

