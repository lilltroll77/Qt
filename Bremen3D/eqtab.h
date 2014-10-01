#ifndef EQTAB_H
#define EQTAB_H

#include <QWidget>
#include <complex>
#include "defines.h"
#include "classes/qcustomplot.h"
#include "eqchannel.h"
#include "UDPcommands.h"


//**************************************************************************
class EQTab : public QWidget
{
    Q_OBJECT
public:
    explicit EQTab(QWidget *parent = 0 , Network *udp =0);
    EQChannel *channel[8];
    QCustomPlot *plotMag;
    QCustomPlot *plotPhase;
    void setLinkedFc(double , bool);
    double getLinkedFc();
    void setPreGain(double pregain, bool);
    double getPreGain();

public slots:
void slot_linkchannel(int channel);
void slot_updatePlot(int newChannel);
void updateLinkedFc(bool blocked);
void slot_linkedFcChanged(double fc);
void slot_PreGainChanged(double gain);
void slot_sendPreGain();

private:
    QTabWidget *channelTabs;
    QGridLayout *layout;
    int activeChannel;
    QVector<double> *f;

    QGroupBox *box_linkedFc;
    QVBoxLayout *layout_linkedFc;
    Knob *knob_linkedFc;
    QGroupBox *box_PreGain;
    QVBoxLayout *layout_PreGain;
    Knob *knob_PreGain;
    const double scale=2147483647;

    QByteArray datagram;
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    quint16 *port_XMOS;
    MainTab* main_tab;
};

  // *****************************************************



    /*
private:
QStatusBar *statusbar;
QUdpSocket *udpSocket;
QHostAddress *IP;
quint16 *port;
int current_channel;
EQsettings eqSettings;
QGridLayout *layout;
QByteArray datagram;
QComboBox *channelSelector;

//Delay
double delay[8];
EQSection *eqSection[8];
QVBoxLayout * layout_delay;
Knob *knob_delay;
QGroupBox *groupBoxDelay;
*/


#endif // EQTAB_H


