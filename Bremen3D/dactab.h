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
#include <QCheckBox>
#include "classes/knob.h"
#include "UDPcommands.h"
#include "maintab.h"

//#include "widget.h"
enum IIRBandWidth_t{Normal , F50kHz , F60kHz , F70kHz};
enum FIRfilter_t{FastRolloff_ES9018 , SlowRolloff_ES9018 , Minphase96k , Slow96k , Minphase192k ,Slow192k ,FastRolloff_var};
enum DPLL_BW_t{NO_BW,LOWEST_BW,LOW_BW,MIDLOW_BW,MID_BW,MIDHIGH_BW,HIGH_BW,HIGHEST_BW,AUTO};
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
      int FIRrolloff;
      enum bool_t UseDeemphaseFilter;
      enum DPLL_BW_t DPPL_BW;
      enum bool_t DPLL_BWx128;
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
    QComboBox *DPLLbox;
    QLabel *DPLLlabel;
    QCheckBox *DPLL_X128;
    QPushButton *buttonLock;
    void setLockAndFs(bool state, int new_fs);
    void setFIRFilter(int settings);
    void setDPLL_BW(enum DPLL_BW_t settings);
    void setDPLL_128X(bool state);
    void updateStopFreq();

signals:

public slots:
    void slot_sendDACsettings();
    void slot_Filterchanged(int type);
    void slot_DPLLchanged(int val);
    void slot_DPPL_x128changed(bool val);
    void slot_FilterFreqChanged(double i);

private:
QGridLayout *layout;
QVBoxLayout *layoutReconstruct;
QVBoxLayout *layoutDPLL;
QGroupBox *groupBoxReconstruct;
QGroupBox *groupBoxDPLL;
QGroupBox *groupBox;
QGridLayout *topLayout;
QLabel *IIRlabel;
QLabel *FIRlabel;
Knob *FIRKnob;
QByteArray datagram;
QUdpSocket  *UDP_Socket;
QHostAddress *IP_XMOS;
quint16 *port_XMOS;
MainTab *main_tab;
int datagram_count=0;
int fs=-1;
const double stop[6]={0.1875   , 0.222222 , 0.291667 , 0.361111 , 0.5 , 0.551};
const double pass[6]={0.104427 , 0.139236 , 0.208854 , 0.278472 ,  0.417708 , 0.4535 };
};

#endif // DACTAB_H
