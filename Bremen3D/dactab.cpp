#include "dactab.h"
#include "mainwindow.h"


DACGain::DACGain(QWidget *parent ,  Network *udp) :
    QWidget(parent)
{

    UDP_Socket =    udp->UDP_Socket;
    IP_XMOS =       udp->IP_XMOS;
    port_XMOS =     udp->port_XMOS;

    topLayout = new QVBoxLayout;
    layout = new QVBoxLayout;
    groupBoxChannel = new QGroupBox;

    channelAlias = new QLineEdit;
    channelAlias->setToolTip(tr("Channel Alias name"));

    knob = new Knob;
    knob->setRange(-40,0,80);
    knob->setTitle("Gain [dB]");
    knob->setKnobColor("rgb(255, 255, 70)");
    knob->setDecimals(1);
    knob->setSingleStep(0.5);

    muteButton = new QPushButton;
    invertButton = new QPushButton;

    muteButton->setCheckable(true);
    muteButton->setText(tr("MUTE"));

    invertButton->setCheckable(true);
    invertButton->setText(tr("Invert"));
    invertButton->setToolTip(tr("DAC Output Polarity"));

    layout->addWidget(channelAlias);
    layout->addWidget(knob);
    layout->addWidget(invertButton);
    layout->addWidget(muteButton);
    groupBoxChannel->setLayout(layout);
    groupBoxChannel->setMaximumHeight(250);
    groupBoxChannel->setMaximumWidth(80);
    groupBoxChannel->setSizePolicy( QSizePolicy::Maximum,QSizePolicy::Maximum );
    groupBoxChannel->setContentsMargins(2,2,2,2);
    topLayout->addWidget(groupBoxChannel);

//SIGNALS
    connect(muteButton      , SIGNAL(toggled(bool))         , this , SLOT(slot_muteButton_Clicked(bool)));
    connect(invertButton    , SIGNAL( toggled(bool))        , this , SLOT(slot_invertButton_Clicked(bool)));
    connect(knob            , SIGNAL(valueChanged(double))  , this , SLOT(slot_gainChanged(double)));

    setLayout(topLayout);
}


void DACGain::setBoxTitle(const QString & title){
    groupBoxChannel->setTitle(title);
}
void DACGain::setChannelAlias(const QString & text){
    channelAlias->setText(text);
}
void DACGain::setChannelID(int ID){
    channelID=ID;
}

void DACGain::slot_muteButton_Clicked(bool state){
     if(state)
       {
         muteButton->setText(tr("MUTED"));
         muteButton->setStyleSheet("color: rgb(255, 0, 0)");
       }else{
         muteButton->setText(tr("MUTE"));
         muteButton->setStyleSheet("color: rgb(0, 0, 0)");
       }
     datagram.clear();
     datagram[0]= MUTE_CHANNEL;
     datagram[1]= state;
     datagram[2]=channelID;
     WRITEDATAGRAM
}

void DACGain::slot_invertButton_Clicked(bool state){
    if(state)
       {
           invertButton->setText(tr("Inverted"));
           invertButton->setStyleSheet("color: rgb(0, 0, 255)");
        }else{
           invertButton->setText(tr("Invert"));
           invertButton->setStyleSheet("color: rgb(0, 0, 0)");
    }
    datagram.clear();
    datagram[0]= INVERT_DAC_CHANNEL;
    datagram[1]= state;
    datagram[2]=channelID;
    WRITEDATAGRAM

}

 void DACGain::slot_gainChanged(double gain){
     datagram.clear();
     datagram[0]= DAC_OUTPUTGAIN;
     datagram[1]= (uchar) round(-2*gain);
     datagram[2]=channelID;
     WRITEDATAGRAM
 }


//***************************************************************************'

DACTab::DACTab(QWidget *parent ,  Network *udp) :
    QWidget(parent)
{
    QString aliasText[8] ={"Left" , "Right" , "Center" , "Rear"};

    topLayout = new QGridLayout;
    layout = new QGridLayout;
    groupBox = new QGroupBox;

    for(int i=0 ; i<8 ; i++){
        channel[i] = new DACGain(this , udp);
        channel[i]->setBoxTitle(QString("Ch %1").arg(i));
        channel[i]->setChannelAlias(aliasText[i]);
        channel[i]->setChannelID(i);
        layout->addWidget(channel[i],0,i);
    }
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    groupBox->setLayout(layout);
    groupBox->setContentsMargins(0,0,0,0);
    groupBox->setTitle("Individual DAC channel settings");
    topLayout->addWidget(groupBox,1,0,1,4);
    groupBoxReconstruct = new QGroupBox;
    groupBoxReconstruct->setTitle(tr("Reconstruction filter"));
    groupBoxReconstruct->setToolTip(tr("The settings of the reconstruction filter of the DAC"));

    IIRlabel = new QLabel;
    IIRlabel->setText("IIR filter");
    IIRFilter = new QComboBox;
    IIRFilter->addItem("Normal",QVariant(0));
    IIRFilter->addItem("f = 50kHz",QVariant(1));
    IIRFilter->addItem("f = 60kHz",QVariant(2));
    IIRFilter->addItem("f = 70kHz",QVariant(3));
    IIRFilter->setToolTip("IIR Bandwith of reconstruction filter");
    IIRFilter->setMaximumWidth(100);

    FIRlabel = new QLabel;
    FIRlabel->setText("FIR filter");
    FIRFilter = new QComboBox;
    FIRFilter->addItem("Fast Rolloff",QVariant(0));
    FIRFilter->addItem("Slow Rolloff",QVariant(1));
    FIRFilter->setToolTip("FIR rolloff of reconstruction filter");
    FIRFilter->setMaximumWidth(100);
    layoutReconstruct = new QVBoxLayout;
    layoutReconstruct->addWidget(IIRlabel);
    layoutReconstruct->addWidget(IIRFilter);
    layoutReconstruct->addWidget(FIRlabel);
    layoutReconstruct->addWidget(FIRFilter);
    groupBoxReconstruct->setLayout(layoutReconstruct);
    topLayout->addWidget(groupBoxReconstruct,0,0,1,1);
    setLayout(topLayout);
}


