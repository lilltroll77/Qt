#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QUdpSocket>
#include <maintab.h>
#include <QGlobalStatic>
#include "eqtab.h"
#include "adctab.h"
#include "dactab.h"
#include "UDPcommands.h"

//Macro
Q_GLOBAL_STATIC(QStatusBar , statusbar)
static EQTab *eq_tab;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0, QStatusBar *statusbar =0);
    int getCurrentTabIndex();



    ~Widget();

private slots:
void readDatagram();

private:
MainTab *main_tab;

DACTab *dac_tab;
ADCTab *adc_tab;
char datagram_RX[DATAGRAM_RX_MAXLEN];
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
};

#endif // WIDGET_H
