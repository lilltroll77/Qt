#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "eqtab.h"
#include "dactab.h"
#include "adctab.h"
#include "maintab.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);
    EQTab *eq_tab;
    MainTab *main_tab;
    DACTab *dac_tab;
    ADCTab *adc_tab;

signals:

public slots:

};

#endif // CENTRALWIDGET_H
