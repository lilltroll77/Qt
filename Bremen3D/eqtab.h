#ifndef EQTAB_H
#define EQTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <QPushButton>
#include <QDial>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSignalMapper>
#include <QUdpSocket>
#include <QStatusBar>
#include "UDPcommands.h"
#include <QVector>
#include "math.h"
#include <QGlobalStatic>
#include "classes/knob.h"
#include "classes/qcustomplot.h"
#include "calcfilt.h"

#define FS 44100
#define FMIN 10
#define SETMAXIMUMWIDTH 80
#define DEFAULT_CHANNEL 0
#define DEFAULT_FC 1000
#define DEFAULT_Q 0.707
#define DEFAULT_GAIN 0
#define DEFAULT_FILTER LowShelf
#define CHANNELS 8
#define SECTIONS 8
#define PLOTSIZE 400

struct EQsettings{
public:
    bool active=false;
    bool link=false;
    FilterType filtertype=DEFAULT_FILTER;
    double fc=DEFAULT_FC;
    double Q=DEFAULT_Q;
    double gain=DEFAULT_GAIN;
    double B[3];
    double A[2];
    std::complex<double> freq[PLOTSIZE];
};

static QCustomPlot *plot;
static QVector< QVector<double> > H;
static QVector<double> f;
static EQsettings eqsettings[CHANNELS][SECTIONS];
static QCPItemTracer *eqTracer[CHANNELS][SECTIONS];
static std::complex<double> ejw[PLOTSIZE];

class EQSection : public QWidget
{
    Q_OBJECT
public:
    explicit EQSection(QWidget *parent = 0 );
    void setPointer2EQSettings(EQsettings *new_eqSettings);
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

signals:

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
 EQsettings *eqSettings; // A pointer to hole EQ settings space
 QByteArray datagram;
 QVBoxLayout *topLayout;
 QVBoxLayout *layout;
 double B[3];
 double A[2];
 std::complex<double> Htot[PLOTSIZE];
 EQsettings *eqsettings_pointer[CHANNELS][SECTIONS]; //debug only


};
 //**********************************************************************************


class EQChannel: public QWidget
{
    Q_OBJECT
public:
    explicit EQChannel(QWidget *parent = 0, int channel=0);
    EQSection *eqSection[8];
    //QCPItemTracer *eqTracer;
    //int getCurrentChannel();
    //void setIP(QHostAddress *newIP);
    //void setPort(quint16 *new_port);
    //void setUDPsocket(QUdpSocket *new_udpSocket);
    //void setStatusBar(QStatusBar *new_statusbar);


signals:

public slots:
//void slot_send_Delay(double new_delay);

private slots:
//void updateEQchannel(int new_channel);

private:

QHBoxLayout * layout_eqsections;
};

//**************************************************************************
class EQTab : public QWidget
{
    Q_OBJECT
public:
    explicit EQTab(QWidget *parent = 0);
    EQChannel *channel[8];

private slots:
void slot_updatePlot(int newChannel);

private:
    QTabWidget *channelTabs;
    QGridLayout *layout;
    QPen *pen[8];
    QColor color;
    int activeChannel=0;
    QScrollArea *area[8];

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


