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
#include "UDPcommands.h"
#include "classes/knob.h"

class MainTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = 0);
    QStatusBar *statusbar;
    //QUdpSocket *udpSocket;
    //QHostAddress IP;
    quint16 *port;
    Knob *knob_bremen3D;

    //Widgets width bublic acess
    QRadioButton *radiobuttons[4];
    QSlider *slider_MasterVolume;
signals:

private slots:
void generateDatagram(int command);

private:
    QStatusBar *toplevel_statusbar;
    QByteArray datagram;
    QSignalMapper *signal_mapper;
    QGridLayout *top_layout;

    //Volume box
    QVBoxLayout *layout_volume;
    QGroupBox *box_volume;
    QPushButton *buttonMute;
    QAction *muteButton_ClickedAction;

    //EthernetBox
    QGroupBox *box_ethernet;
    QGridLayout *ethernetLayout;
    QPushButton *buttonSyncToXMOS;
    QPushButton *buttonSyncFromXMOS;
    QPushButton *buttonPingXMOS;
    QLineEdit *lineEditXMOSIP;

    //Program Box
    QGridLayout *programLayout;
    QGroupBox *box_program;
    QVBoxLayout *box_layout;

    QPushButton *openbuttons[4];
    QPushButton *savebuttons[4];
    //Input selector box
    QGroupBox *box_input;
    QComboBox *input_selector;

    //Crossover box
    QGroupBox *box_crossover;
    Knob *knob_crossover;
    QVBoxLayout *crossoverLayout;

    //Mixer box
    QGroupBox *box_mixer;

    QVBoxLayout *mixerLayout;

};

#endif // MAINTAB_H
