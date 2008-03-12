#ifndef __SCALEDIALOG_H
#define __SCALEDIALOG_H

#include <QtGui/QDialog>
class QSpinBox;
class QComboBox;

class ScaleDialog : public QDialog
{
public:
    ScaleDialog(int w, int h, bool showFilters, QWidget *parent=0);
    int resultWidth();
    int resultHeight();
    int filter();
private:
    QSpinBox *wEdit, *hEdit;
    QComboBox *filterBox;
};

#endif

