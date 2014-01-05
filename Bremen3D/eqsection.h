#ifndef EQSECTION_H
#define EQSECTION_H

#include <complex>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <classes/knob.h>
#include "classes/qcustomplot.h"
#include "defines.h"
#include "calcfilt.h"
#include "UDPcommands.h"
//enum FilterType{LowShelf , HighSelf , PeakingEQ , Notch , AllPass , HighPass , LowPass , BandPass };

class EQSection : public QWidget
{
    Q_OBJECT
public:
    explicit EQSection(QWidget *parent = 0 , QCustomPlot *new_plot = 0);
    void setSectionID(int newID);
    void setChannelID(int newID);
    void setBoxTitle(const QString &title);
    void updateSettingsAndPlot();
    QGroupBox *groupBox;
    QComboBox *filterType;
    QCheckBox *link;
    Knob *knob_fc;
    Knob *knob_Q;
    Knob *knob_gain;
    double B[3];
    double A[2];
    std::complex<double> freq[PLOTSIZE];
    QCPItemTracer *eqTracer;

signals:
    void eqchanged();

private slots:
void slot_gainChanged(double gain);
void slot_Q_Changed(double Q);
void slot_fcChanged(double fc);
void slot_linkStatusChanged(bool state);
void slot_filtertypeChanged(int type);
void slot_activeEQChanged(bool state);

private:
 quint16 *port;
 int channelID;
 int sectionID;
 QByteArray datagram;
 QVBoxLayout *topLayout;
 QVBoxLayout *layout;
 QCustomPlot *plot;
};



#endif // EQSECTION_H
