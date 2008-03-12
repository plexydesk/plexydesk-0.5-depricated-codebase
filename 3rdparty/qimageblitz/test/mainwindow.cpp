#include "mainwindow.h"
#include "scaledialog.h"
#include <qimageblitz.h>

#include <QLabel>
#include <QImage>
#include <QScrollArea>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QMenuBar>
#include <QInputDialog>

BlitzMainWindow::BlitzMainWindow()
    : QMainWindow()
{
    setWindowTitle(tr("Blitz Effect Test"));
    QScrollArea *sa = new QScrollArea(this);
    lbl = new QLabel;
    sa->setWidget(lbl);
    setCentralWidget(sa);

    QToolBar *tBar = addToolBar(tr("Effect Options"));
    QAction *openAct = tBar->addAction(tr("Open"), this, SLOT(slotOpen()));
    QAction *revertAct = tBar->addAction(tr("Revert"), this, SLOT(slotRevert()));
    qualityCB = new QCheckBox(tr("High Quality"), tBar);
    qualityCB->setChecked(true);
    tBar->addWidget(qualityCB);

    QMenuBar *mBar = menuBar();
    QMenu *fileMnu = mBar->addMenu(tr("File"));
    fileMnu->addAction(openAct);
    fileMnu->addAction(revertAct);
    fileMnu->addSeparator();
    fileMnu->addAction(tr("Close"), this, SLOT(close()));

    QMenu *effectMnu = mBar->addMenu(tr("Effects"));
    effectMnu->addAction(tr("Grayscale (MMX)"), this, SLOT(slotGrayscale()));
    effectMnu->addAction(tr("Invert (MMX)"), this, SLOT(slotInvert()));

    QMenu *subMnu = effectMnu->addMenu(tr("Threshold"));
    subMnu->addAction(tr("Threshold (Default)"), this, SLOT(slotThreshold()));
    subMnu->addAction(tr("Threshold (Red)"), this, SLOT(slotThresholdRed()));
    subMnu->addAction(tr("Threshold (Green)"), this, SLOT(slotThresholdGreen()));
    subMnu->addAction(tr("Threshold (Blue)"), this, SLOT(slotThresholdBlue()));
    subMnu->addAction(tr("Threshold (Alpha)"), this, SLOT(slotThresholdAlpha()));
    subMnu = effectMnu->addMenu(tr("Intensity (MMX)"));
    subMnu->addAction(tr("Increment Intensity"), this, SLOT(slotIncIntensity()));
    subMnu->addAction(tr("Decrement Intensity"), this, SLOT(slotDecIntensity()));
    subMnu->addSeparator();
    subMnu->addAction(tr("Increment Red Intensity"), this, SLOT(slotIncRedIntensity()));
    subMnu->addAction(tr("Decrement Red Intensity"), this, SLOT(slotDecRedIntensity()));
    subMnu->addAction(tr("Increment Green Intensity"), this, SLOT(slotIncGreenIntensity()));
    subMnu->addAction(tr("Decrement Green Intensity"), this, SLOT(slotDecGreenIntensity()));
    subMnu->addAction(tr("Increment Blue Intensity"), this, SLOT(slotIncBlueIntensity()));
    subMnu->addAction(tr("Decrement Blue Intensity"), this, SLOT(slotDecBlueIntensity()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Spiff"), this, SLOT(slotSpiff()));
    effectMnu->addAction(tr("Dull"), this, SLOT(slotDull()));
    effectMnu->addSeparator();
    effectMnu->addAction(tr("Desaturate"), this, SLOT(slotDesaturate()));
    effectMnu->addAction(tr("Fade"), this, SLOT(slotFade()));
    effectMnu->addAction(tr("Flatten"), this, SLOT(slotFlatten()));
    effectMnu->addSeparator();
    effectMnu->addAction(tr("Sharpen"), this, SLOT(slotSharpen()));
    effectMnu->addAction(tr("Blur"), this, SLOT(slotBlur()));
    effectMnu->addAction(tr("Gaussian Sharpen (MMX)"), this, SLOT(slotGaussianSharpen()));
    effectMnu->addAction(tr("Gaussian Blur (MMX)"), this, SLOT(slotGaussianBlur()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Sobel Edge (MMX)"), this, SLOT(slotEdge()));
    effectMnu->addAction(tr("Convolve Edge (MMX)"), this, SLOT(slotConvolveEdge()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Equalize"), this, SLOT(slotEqualize()));
    effectMnu->addAction(tr("Normalize"), this, SLOT(slotNormalize()));
    effectMnu->addAction(tr("Despeckle"), this, SLOT(slotDespeckle()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Charcoal"), this, SLOT(slotCharcoal()));
    effectMnu->addAction(tr("Swirl"), this, SLOT(slotSwirl()));
    effectMnu->addAction(tr("Implode"), this, SLOT(slotImplode()));
    effectMnu->addAction(tr("Wave"), this, SLOT(slotWave()));
    effectMnu->addAction(tr("Oil Paint"), this, SLOT(slotOilpaint()));
    effectMnu->addAction(tr("Emboss"), this, SLOT(slotEmboss()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Smoothscale (MMX)..."), this, SLOT(slotSmoothscale()));
    effectMnu->addAction(tr("Smoothscale Filtered..."), this, SLOT(slotSmoothscaleFiltered()));
    effectMnu->addSeparator();
    effectMnu->addAction(tr("Modulate..."), this, SLOT(slotModulate()));
    effectMnu->addSeparator();

    effectMnu->addAction(tr("Gradient..."), this, SLOT(slotGradient()));
    effectMnu->addAction(tr("Unbalanced Gradient..."), this, SLOT(slotUnbalancedGradient()));
    effectMnu->addAction(tr("8bpp Gradient..."), this, SLOT(slotGrayscaleGradient()));
    effectMnu->addAction(tr("8bpp Unbalanced Gradient..."), this, SLOT(slotGrayscaleUnbalancedGradient()));

    resize(sizeHint());
}

void BlitzMainWindow::openFile(const QString &filename)
{
    setWindowTitle(filename);
    QImage tmpImg(filename);
    if(tmpImg.isNull()){
        QMessageBox::warning(this, tr("File Open Error"),
                             tr("Unable to open image."));
        return;
    }
    img = tmpImg;
    qWarning("Loaded image: %s, (%dx%d), format: %d", (const char *)
             filename.toLatin1(), img.width(), img.height(), img.format());
    fn = filename;
    lbl->setPixmap(QPixmap::fromImage(img));
    if(lbl->sizeHint() != lbl->size())
        lbl->resize(lbl->sizeHint());
    else
        lbl->update();
}

void BlitzMainWindow::slotOpen()
{
    QString filename(QFileDialog::getOpenFileName(this));
    if(!filename.isEmpty())
        openFile(filename);
}

void BlitzMainWindow::slotRevert()
{
    if(!fn.isEmpty())
        openFile(fn);
}

void BlitzMainWindow::slotGrayscale()
{
    Blitz::grayscale(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotInvert()
{
    Blitz::invert(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotEqualize()
{
    Blitz::equalize(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotNormalize()
{
    Blitz::normalize(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDespeckle()
{
    Blitz::despeckle(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotSharpen()
{
    img = Blitz::sharpen(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotGaussianSharpen()
{
    img = Blitz::gaussianSharpen(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotSpiff()
{
    Blitz::contrast(img, true, 3);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDull()
{
    Blitz::contrast(img, false, 3);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDesaturate()
{
    Blitz::desaturate(img, 0.5);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotFade()
{
    Blitz::fade(img, 0.5, Qt::blue);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotFlatten()
{
    Blitz::flatten(img, Qt::yellow, Qt::blue);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotBlur()
{
    img = Blitz::blur(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotGaussianBlur()
{
    img = Blitz::gaussianBlur(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotEdge()
{
    img = Blitz::edge(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotConvolveEdge()
{
    img = Blitz::convolveEdge(img, 0.0, qualityCB->isChecked() ?
                              Blitz::High : Blitz::Low);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotCharcoal()
{
    img = Blitz::charcoal(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotSwirl()
{
    img = Blitz::swirl(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotImplode()
{
    img = Blitz::implode(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotWave()
{
    img = Blitz::wave(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotOilpaint()
{
    img = Blitz::oilPaint(img, 0.0, qualityCB->isChecked() ?
                          Blitz::High : Blitz::Low);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotEmboss()
{
    img = Blitz::emboss(img, 0.0, 1.0, qualityCB->isChecked() ?
                          Blitz::High : Blitz::Low);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotAntialias()
{
    img = Blitz::antialias(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotSmoothscale()
{
    ScaleDialog dlg(img.width(), img.height(), false, this);
    if(dlg.exec() == QDialog::Accepted){
        img = Blitz::smoothScale(img, dlg.resultWidth(), dlg.resultHeight());
        lbl->setPixmap(QPixmap::fromImage(img));
        lbl->resize(lbl->sizeHint());
    }
}

void BlitzMainWindow::slotSmoothscaleFiltered()
{
    ScaleDialog dlg(img.width(), img.height(), true, this);
    if(dlg.exec() == QDialog::Accepted){
        img = Blitz::smoothScaleFilter(img, dlg.resultWidth(),
                                       dlg.resultHeight(), 1.0,
                                       (Blitz::ScaleFilterType)dlg.filter());
        lbl->setPixmap(QPixmap::fromImage(img));
        lbl->resize(lbl->sizeHint());
    }
}

void BlitzMainWindow::slotModulate()
{
    QString filename(QFileDialog::getOpenFileName(this, tr("Select image to modulate")));
    if(filename.isEmpty())
        return;
    QImage modImg(filename);
    if(modImg.isNull()){
        QMessageBox::warning(this, tr("File Open Error"),
                             tr("Unable to open image."));
        return;
    }

    QStringList types;
    types << tr("Intensity") << tr("Saturation") << tr("Hue") << tr("Contrast");
    bool ok;
    QString typeStr(QInputDialog::getItem(this, tr("Select modulation type"),
                                          tr("Modulation type:"), types, 0,
                                          false, &ok));
    if(!ok)
        return;

    Blitz::ModulationType type;
    if(typeStr == tr("Intensity"))
        type = Blitz::Intensity;
    else if(typeStr == tr("Saturation"))
        type = Blitz::Saturation;
    else if(typeStr == tr("Hue"))
        type = Blitz::HueShift;
    else
        type = Blitz::Contrast;

    img = Blitz::modulate(img, modImg, false, type, 200, Blitz::All);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotGradient()
{
    QStringList types;
    types << tr("Vertical") << tr("Horizontal") << tr("Diagonal") <<
        tr("Cross Diagonal") << tr("Pyramid") << tr("Rectangle") <<
        tr("Pipe Cross") << tr("Elliptic");
    bool ok;
    QString typeStr(QInputDialog::getItem(this, tr("Select gradient type"),
                                          tr("Gradient type:"), types, 0,
                                          false, &ok));
    if(!ok)
        return;
    img = Blitz::gradient(img.size(), Qt::white, Qt::black,
                          (Blitz::GradientType)types.indexOf(typeStr));
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotUnbalancedGradient()
{
    QStringList types;
    types << tr("Vertical") << tr("Horizontal") << tr("Diagonal") <<
        tr("Cross Diagonal") << tr("Pyramid") << tr("Rectangle") <<
        tr("Pipe Cross") << tr("Elliptic");
    bool ok;
    QString typeStr(QInputDialog::getItem(this, tr("Select gradient type"),
                                          tr("Gradient type:"), types, 0,
                                          false, &ok));
    if(!ok)
        return;
    img = Blitz::unbalancedGradient(img.size(), Qt::white, Qt::black,
                                    (Blitz::GradientType)types.indexOf(typeStr));
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotGrayscaleGradient()
{
    QStringList types;
    types << tr("Vertical") << tr("Horizontal") << tr("Diagonal") <<
        tr("Cross Diagonal") << tr("Pyramid") << tr("Rectangle") <<
        tr("Pipe Cross") << tr("Elliptic");
    bool ok;
    QString typeStr(QInputDialog::getItem(this, tr("Select gradient type"),
                                          tr("Gradient type:"), types, 0,
                                          false, &ok));
    if(!ok)
        return;
    img = Blitz::grayGradient(img.size(), 0xFF, 0x00,
                              (Blitz::GradientType)types.indexOf(typeStr));
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotGrayscaleUnbalancedGradient()
{
    QStringList types;
    types << tr("Vertical") << tr("Horizontal") << tr("Diagonal") <<
        tr("Cross Diagonal") << tr("Pyramid") << tr("Rectangle") <<
        tr("Pipe Cross") << tr("Elliptic");
    bool ok;
    QString typeStr(QInputDialog::getItem(this, tr("Select gradient type"),
                                          tr("Gradient type:"), types, 0,
                                          false, &ok));
    if(!ok)
        return;
    img = Blitz::grayUnbalancedGradient(img.size(), 0xFF, 0x00,
                                        (Blitz::GradientType)types.indexOf(typeStr));
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotThreshold()
{
    img = Blitz::threshold(img);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotThresholdRed()
{
    img = Blitz::threshold(img, 127, Blitz::Red);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotThresholdGreen()
{
    img = Blitz::threshold(img, 127, Blitz::Green);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotThresholdBlue()
{
    img = Blitz::threshold(img, 127, Blitz::Blue);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotThresholdAlpha()
{
    img = Blitz::threshold(img, 127, Blitz::Alpha);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotIncIntensity()
{
    Blitz::intensity(img, 0.5);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDecIntensity()
{
    Blitz::intensity(img, -0.5);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotIncRedIntensity()
{
    Blitz::channelIntensity(img, 0.5, Blitz::Red);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDecRedIntensity()
{
    Blitz::channelIntensity(img, -0.5, Blitz::Red);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotIncGreenIntensity()
{
    Blitz::channelIntensity(img, 0.5, Blitz::Green);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDecGreenIntensity()
{
    Blitz::channelIntensity(img, -0.5, Blitz::Green);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotIncBlueIntensity()
{
    Blitz::channelIntensity(img, 0.5, Blitz::Blue);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

void BlitzMainWindow::slotDecBlueIntensity()
{
    Blitz::channelIntensity(img, -0.5, Blitz::Blue);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->update();
}

#include "mainwindow.moc"

