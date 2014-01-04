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



class DACGain: public QWidget
{
    Q_OBJECT
public:
    explicit DACGain(QWidget *parent = 0);
    void setBoxTitle(const QString &title);
    void setChannelAlias(const QString & text);
    void setChannelID(int ID);

signals:
    void gainChanged(double);
    void muteButton_Clicked(bool);
    void invertButton_Clicked(bool);

public slots:
    void slot_muteButton_Clicked(bool state);
    void slot_invertButton_Clicked(bool state);
    void slot_gainChanged(double gain);

private:
int channelID;
QByteArray datagram;
QVBoxLayout *topLayout;
QVBoxLayout *layout;
QLineEdit *channelAlias;
QPushButton *muteButton;
QPushButton *invertButton;
QGroupBox *groupBoxChannel;
Knob *knob;
};

class DACTab : public QWidget
{
    Q_OBJECT
public:
    explicit DACTab(QWidget *parent = 0);
    QStatusBar *statusbar;

signals:

public slots:

private:
QGridLayout *layout;
QGroupBox *groupBoxReconstruct;
QGroupBox *groupBox;
QGridLayout *topLayout;
};

#endif // DACTAB_H
