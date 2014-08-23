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


class MainTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = 0 ,  Network *udp =0);
    QStatusBar *statusbar;
    //Knob *knob_bremen3D;

    //Widgets width public acess
    QRadioButton *radiobuttons[4];
    QSlider *slider_MasterVolume;
signals:

private slots:
void slot_volumesliderChanged(int value);
void slot_volumeSpinboxChanged(int value);
void slot_mutebuttonToggled(bool state);
void slot_inputselectorChanged(int index);
void slot_synctoXMOS();
void slot_syncFromXMOS();
void slot_pingXMOS();
void slot_programChanged(int index);
//void slot_effectChanged(double value);

private:
    int currentProgram;
    QStatusBar *toplevel_statusbar;
    QSignalMapper *signal_mapper;
    QGridLayout *top_layout;

    //Volume box
    QVBoxLayout *layout_volume;
    QGroupBox *box_volume;
    QPushButton *buttonMute;
    QAction *muteButton_ClickedAction;
    QSpinBox *spinbox_volume;

    //EthernetBox
    QGroupBox *box_ethernet;
    QGridLayout *ethernetLayout;
    QPushButton *buttonSyncToXMOS;
    QPushButton *buttonSyncFromXMOS;
    QPushButton *buttonPingXMOS;
    QLineEdit *lineEditXMOSIP;

    //Program selector box
    QGroupBox   *program_Groupbox;
    QVBoxLayout *program_layout;
    QComboBox   *program_ComboBox;


    //Input selector box
    QGroupBox   *input_Groupbox;
    QVBoxLayout *input_layout;
    QComboBox   *input_ComboBox;

    //UDP
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    quint16 *port_XMOS;
    QByteArray datagram;

};

#endif // MAINTAB_H
