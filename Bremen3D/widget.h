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
#include "maintab.h"
#include "mixertab.h"
#include "UDPcommands.h"



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    int getCurrentTabIndex();
    MixerTab *mixer_tab;
    MainTab *main_tab;
    EQTab *eq_tab;
    DACTab *dac_tab;
    ADCTab *adc_tab;
    Network udp;


    ~Widget();

public slots:

private slots:
    void readDatagram();

private:
char datagram_RX[DATAGRAM_RX_MAXLEN];
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QByteArray datagram;
    quint16 *port;
    QHostAddress *IP_RX;
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    QHostAddress *IP_LOCAL;
    quint16 *port_XMOS;
    DAC_settings_t *DAC;
    EQ_channel_t *EQ;
    XMOSMixer_settings_t *XMOSMixer;
    QStatusBar *statusbar;

};

#endif // WIDGET_H
