#ifndef EQTAB_H
#define EQTAB_H

#include <QWidget>
#include <complex>
#include "defines.h"
#include "classes/qcustomplot.h"
#include "eqchannel.h"


//**************************************************************************
class EQTab : public QWidget
{
    Q_OBJECT
public:
    explicit EQTab(QWidget *parent = 0);
    EQChannel *channel[8];

    QCustomPlot *plot;

public slots:
void slot_linkchannel(int channel);
void slot_updatePlot(int newChannel);

private:
    QTabWidget *channelTabs;
    QGridLayout *layout;
    int activeChannel;
    QVector<double> *f;
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


