#ifndef EQCHANNEL_H
#define EQCHANNEL_H

#include <QWidget>
#include <QVector>
#include "classes/knob.h"
#include "classes\qcustomplot.h"
#include "calcfilt.h"
#include "eqsection.h"
#include "defines.h"

class EQChannel : public QWidget
{
    Q_OBJECT
public:
    explicit EQChannel(QWidget *parent = 0 , int new_channel=0 , QCustomPlot *plot_ref=0 , Network *udp=0);
    void enableGraph();
    void disableGraph();
    EQSection *eqSection[8];
    QPen *pen;
    QCheckBox *linkChannel[8];
    QLabel *label_link;
    QScrollArea *scrollArea;
    Knob *knob_delay;
    QGroupBox *box_delay;


signals:

public slots:
void recalc_graph();
void slot_delayChanged(double delay);


private:
QHBoxLayout * layout_eqsections;
QHBoxLayout * layout_delay;
std::complex<double> Htot[PLOTSIZE];
QVector<double> Hmag;
QCustomPlot *plot;
int channel;
QColor color;
QWidget *widget;
QByteArray datagram;
QUdpSocket  *UDP_Socket;
QHostAddress *IP_XMOS;
};

#endif // EQCHANNEL_H

