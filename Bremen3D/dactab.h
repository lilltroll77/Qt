#ifndef DACTAB_H
#define DACTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QDial>
#include <QStatusBar>
#include <QUdpSocket>
#include <QComboBox>
#include "classes/knob.h"
#include "UDPcommands.h"
#include "maintab.h"

//#include "widget.h"
enum IIRBandWidth_t{Normal , F50kHz , F60kHz , F70kHz};
enum FIRfilter_t{FastRolloff,SlowRolloff,userDefined};
enum DPPL_BW_t{NO_BW,LOWEST_BW,LOW_BW,MEDLOW_BW,MEDIUM_BW,MEDHIGH_BW,HIGH_BW,HIGEST_BW};
enum polarity_t{InPhase,AntiPhase};
enum mute_t{unmuted , muted};
enum bool_t{False=0 , True=1};

typedef struct{
      enum mute_t mute;
      enum polarity_t polarity;
      int MasterTrim;
      //int FIRStage1[64];
      //int FIRStage2[16];
      unsigned char Gain;
  }DACChannel_t;

typedef struct{
      DACChannel_t channel[8];
      unsigned char MasterVolume;
      enum bool_t MuteAll;
      enum IIRBandWidth_t IIRfilter;
      enum FIRfilter_t FIRrolloff;
      enum bool_t UseDeemphaseFilter;
      enum DPPL_BW_t DPPL_BW;
      enum bool_t DPPL_BWx128;
}DAC_settings_t;

class DACGain: public QWidget
{
    Q_OBJECT
public:
    explicit DACGain(QWidget *parent = 0, Network *udp = 0);
    void setBoxTitle(const QString &title);
    void setChannelAlias(const QString & text);
    void setChannelID(int ID);
    QLineEdit *channelAlias;
    mute_t getMute();
    void setMute(mute_t , bool);
    polarity_t getPolarity();
    double getGain();

    void setPolarity(polarity_t , bool);
    void setGain(double , bool);

signals:

public slots:
    void slot_muteButton_Clicked(bool state);
    void slot_invertButton_Clicked(bool state);
    void slot_gainChanged(double gain);

private:
int channelID;
QVBoxLayout *topLayout;
QVBoxLayout *layout;
QGroupBox *groupBoxChannel;
QUdpSocket  *UDP_Socket;
QHostAddress *IP_XMOS;
quint16 *port_XMOS;
QByteArray datagram;
QPushButton *muteButton;
QPushButton *invertButton;
Knob *knob;
MainTab* main_tab;

};

class DACTab : public QWidget
{
    Q_OBJECT
public:
    explicit DACTab(QWidget *parent = 0 , Network *udp =0);
    QStatusBar *statusbar;
    DACGain *channel[8];
    QComboBox *IIRFilter;
    QComboBox *FIRFilter;
    QLabel *DPLL;

signals:

public slots:
    void slot_sendDACsettings();
    void slot_FIRchanged(int val);
    void slot_IIRchanged(int val);

private:
QGridLayout *layout;
QVBoxLayout *layoutReconstruct;
QGroupBox *groupBoxReconstruct;
QGroupBox *groupBox;
QGridLayout *topLayout;
QLabel *IIRlabel;
QLabel *FIRlabel;
QByteArray datagram;
QUdpSocket  *UDP_Socket;
QHostAddress *IP_XMOS;
quint16 *port_XMOS;
MainTab *main_tab;
int datagram_count=0;
};

#endif // DACTAB_H
