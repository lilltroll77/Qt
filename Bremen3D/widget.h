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

enum IIRBandWidth_t{Normal , F50kHz , F60kHz , F70kHz};
enum FIRfilter_t{FastRolloff,SlowRolloff,userDefined};
enum bool_t{True,False};
enum DPPL_BW_t{NO_BW,LOWEST_BW,LOW_BW,MEDLOW_BW,MEDIUM_BW,MEDHIGH_BW,HIGH_BW,HIGEST_BW};


typedef struct{
      enum bool_t mute;
      int MasterTrim;
      //int FIRStage1[64];
      //int FIRStage2[16];
      char Gain;
  }DACChannel_t;

typedef struct{
      DACChannel_t channel[8];
      char MasterVolume;
      char Polarity;
      enum bool_t MuteAll;
      enum IIRBandWidth_t IIRfilter;
      enum FIRfilter_t FIRrolloff;
      enum bool_t UseDeemphaseFilter;
      enum DPPL_BW_t DPPL_BW;
      enum bool_t DPPL_BWx128;
}DAC_settings_t;





class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0, QStatusBar *statusbar =0);
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
    Mixer_settings_t *Mixer;


};

#endif // WIDGET_H
