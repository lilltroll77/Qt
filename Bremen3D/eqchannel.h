#ifndef EQCHANNEL_H
#define EQCHANNEL_H

#include <QWidget>
#include <QVector>
#include "classes/knob.h"
#include "classes\qcustomplot.h"
#include "calcfilt.h"
#include "eqsection.h"
#include "defines.h"
#include <complex.h>

class EQChannel : public QWidget
{
    Q_OBJECT
public:
    explicit EQChannel(QWidget *parent = 0 , int new_channel=0 , QCustomPlot *plot_ref=0 , Network *udp=0 , Knob *knob_linkedFc=0);
    void enableGraph();
    void disableGraph();
    EQSection *eqSection[8];
    QPen *pen;
    QCheckBox *linkChannel[8];
    QLabel *label_link;
    QScrollArea *scrollArea;

    void setDelay(double , bool);
    double getDelay();


signals:

public slots:
void recalc_graph();
void slot_delayChanged(double delay);
void slot_sendEQChannelSettings();

private:

QGroupBox *box_delay;
QHBoxLayout * layout_eqsections;
QHBoxLayout * layout_delay;
std::complex<double> Htot[PLOTSIZE];
QVector<double> Hmag;
QVector<double> Hdeg;
QCustomPlot *plot;
int channel;
QColor color;
QWidget *widget;
QByteArray datagram;
QUdpSocket  *UDP_Socket;
QHostAddress *IP_XMOS;
Knob *knob_delay;
MainTab* main_tab;


};

#endif // EQCHANNEL_H

