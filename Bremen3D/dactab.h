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
#include "classes/knob.h"
#include "UDPcommands.h"


class DACGain: public QWidget
{
    Q_OBJECT
public:
    explicit DACGain(QWidget *parent = 0, Network *udp = 0);
    void setBoxTitle(const QString &title);
    void setChannelAlias(const QString & text);
    void setChannelID(int ID);
    QPushButton *muteButton;
    QPushButton *invertButton;
    Knob *knob;
    QLineEdit *channelAlias;


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

};

class DACTab : public QWidget
{
    Q_OBJECT
public:
    explicit DACTab(QWidget *parent = 0 , Network *udp =0);
    QStatusBar *statusbar;
    DACGain *channel[8];

signals:

public slots:

private:
QGridLayout *layout;
QGroupBox *groupBoxReconstruct;
QGroupBox *groupBox;
QGridLayout *topLayout;
};

#endif // DACTAB_H
