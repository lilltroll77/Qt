#include "dactab.h"
#include "mainwindow.h"


DACGain::DACGain(QWidget *parent) :
    QWidget(parent)
{
    topLayout = new QVBoxLayout;
    layout = new QVBoxLayout;
    groupBoxChannel = new QGroupBox;

    channelAlias = new QLineEdit;
    channelAlias->setToolTip(tr("Channel Alias name"));

    knob = new Knob;
    knob->setRange(-10,10,40);
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

    connect(muteButton , SIGNAL(toggled(bool)) , this , SLOT(slot_muteButton_Clicked(bool)));
    connect(invertButton , SIGNAL( toggled(bool)) , this , SLOT(slot_invertButton_Clicked(bool)));
    connect(knob, SIGNAL(valueChanged(double)) , this , SLOT(slot_gainChanged(double)));

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
    emit muteButton_Clicked(state);
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
    emit invertButton_Clicked(state);
}

 void DACGain::slot_gainChanged(double gain){
     emit gainChanged(gain);
 }


//***************************************************************************'

DACTab::DACTab(QWidget *parent) :
    QWidget(parent)
{
    QString aliasText[8] ={"Left" , "Right" , "Center" , "Rear"};

    topLayout = new QGridLayout;
    layout = new QGridLayout;
    groupBox = new QGroupBox;

    for(int i=0 ; i<8 ; i++){
        channel[i] = new DACGain;
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
    topLayout->addWidget(groupBoxReconstruct,0,0,1,1);
    setLayout(topLayout);
}


