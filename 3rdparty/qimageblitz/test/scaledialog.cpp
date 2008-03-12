#include "scaledialog.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QLayout>

ScaleDialog::ScaleDialog(int w, int h, bool showFilters, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Image Size"));
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Width:"), this));
    wEdit = new QSpinBox(this);
    wEdit->setRange(1, 10000);
    wEdit->setValue(w);
    layout->addWidget(wEdit);

    layout->addWidget(new QLabel(tr("Height:"), this));
    hEdit = new QSpinBox(this);
    hEdit->setRange(1, 10000);
    hEdit->setValue(h);
    layout->addWidget(hEdit);

    if(showFilters){
        layout->addWidget(new QLabel(tr("Filter mode"), this));
        filterBox = new QComboBox(this);
        layout->addWidget(filterBox);
        filterBox->addItem(tr("Point"));
        filterBox->addItem(tr("Box"));
        filterBox->addItem(tr("Triangle"));
        filterBox->addItem(tr("Hermite"));
        filterBox->addItem(tr("Hanning"));
        filterBox->addItem(tr("Hamming"));
        filterBox->addItem(tr("Blackman"));
        filterBox->addItem(tr("Gaussian"));
        filterBox->addItem(tr("Quadratic"));
        filterBox->addItem(tr("Cubic"));
        filterBox->addItem(tr("Catrom"));
        filterBox->addItem(tr("Mitchell"));
        filterBox->addItem(tr("Lanczos"));
        filterBox->addItem(tr("Bessel"));
        filterBox->addItem(tr("Sinc"));
        filterBox->setCurrentIndex(6);
    }
    else
        filterBox = NULL;

    layout->addStretch(1);

    QDialogButtonBox *bbox = new QDialogButtonBox(this);
    connect(bbox->addButton(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(bbox->addButton(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(reject()));
    layout->addWidget(bbox);
}

int ScaleDialog::resultWidth()
{
    return(wEdit->value());
}

int ScaleDialog::resultHeight()
{
    return(hEdit->value());
}

int ScaleDialog::filter()
{
    // 0 is undefined filter
    return(filterBox ? filterBox->currentIndex()+1 : 0);
}


