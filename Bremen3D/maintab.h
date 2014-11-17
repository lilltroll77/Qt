#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QSlider>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QUdpSocket>
#include <QRadioButton>
#include <QLineEdit>
#include <QSignalMapper>
#include <QStatusBar>
#include "classes/knob.h"
#include "UDPcommands.h"
#include <QTimer>
//#include "widget.h"

enum format_t{PCM , IEC61937 , DTS_LD , DTS_CD , DGTL_SIL};


class MainTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = 0 ,  Network *udp_ref =0);
    void setMuteState(bool , bool blocked);
    bool getMuteState();

    void getLock_fs();
    void setMasterVolume(int val, bool blocked);
    int  getMasterVolume();
    void setLock(bool state, int fs);
    void sendSettings();
    void setProgram(int new_program , bool blocked);
    int  getProgram();
    void sendProgram();
    void setInputSelector(int val , bool blocked);
    int getInputSelector();
    void setMode(int new_mode);
    int getMode();
    void setFormat(enum format_t format);
    void setCopy(int copy);
    void setOriginal(int original);
    void setEmphasis(int emph);
    void setPro(int pro);
    void setPLL(int pll);
    void setValidity(int v);


    QStatusBar *statusbar;
    //Knob *knob_bremen3D;

    //Widgets width public acess

signals:

private slots:
void slot_volumesliderChanged(int value);
void slot_volumeSpinboxChanged(int value);
void slot_mutebuttonToggled(bool state);
void slot_inputselectorChanged(int index);
void slot_programChanged(double);

public slots:
void slot_syncFromXMOS();
void slot_sendMute(bool);
void slot_pingXMOS();

private:

    QSlider *slider_MasterVolume;
    QStatusBar *toplevel_statusbar;
    QSignalMapper *signal_mapper;
    QGridLayout *top_layout;

    //Volume box
    QVBoxLayout *layout_volume;
    QGroupBox *box_volume;
    QPushButton *buttonMute;
    QPushButton *buttonLock;
    QLabel *labelFS;
    QAction *muteButton_ClickedAction;
    QSpinBox *spinbox_volume;

    //Input selector box
    QGroupBox   *input_Groupbox;
    QVBoxLayout *input_layout;
    QComboBox   *input_ComboBox;
    QLabel *input_pro;
    QLabel *input_copy;
    QLabel *input_orig;
    QLabel *input_emph;
    QLabel *input_format;
    QLabel *input_PLL;
    QLabel *input_validity;


    //Program selector
    QGroupBox   *program_Groupbox;
    Knob *program_knob;
    QVBoxLayout *program_layout;
    //QPushButton *button_preset;
    int mode=PRESET;

    //UDP
    Network *udp;
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    quint16 *port_XMOS;
    QByteArray datagram;




};

#endif // MAINTAB_H
