#include "dactab.h"
#include "mainwindow.h"
#include <QDebug>


DACGain::DACGain(QWidget *parent ,  Network *udp) :
    QWidget(parent)
{

    main_tab=this->parent()->parent()->parent()->findChild<MainTab*>("MainTab");

    UDP_Socket =    udp->UDP_Socket;
    IP_XMOS =       udp->IP_XMOS;
    port_XMOS =     udp->port_XMOS;

    topLayout = new QVBoxLayout;
    layout = new QVBoxLayout;
    groupBoxChannel = new QGroupBox;

    channelAlias = new QLineEdit;
    channelAlias->setToolTip(tr("Channel Alias name"));
    channelAlias->setMaxLength(12);

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

mute_t DACGain::getMute(){
    if(muteButton->isChecked())
        return muted;
    else
        return unmuted;
}

void DACGain::setGain(double val , bool blocked){
    if(blocked)
        knob->blockSignals(true);
    knob->setValue(val);
    if(blocked)
        knob->blockSignals(false);
    }

void DACGain::setMute(mute_t state , bool blocked){
    if(blocked)
        muteButton->blockSignals(true);
    if(state == muted){
        muteButton->setChecked(true);
        muteButton->setText(tr("MUTED"));
        muteButton->setStyleSheet("color: rgb(255, 0, 0)");
    }
    else if(state == unmuted){
        muteButton->setChecked(false);
        muteButton->setText(tr("MUTE"));
        muteButton->setStyleSheet("color: rgb(0, 0, 0)");
    }
    if(blocked)
        muteButton->blockSignals(false);
}

polarity_t DACGain::getPolarity(){
    if(invertButton->isChecked())
            return AntiPhase;
    else
        return InPhase;
}

void DACGain::setPolarity(polarity_t state , bool blocked){
    if(blocked)
        invertButton->blockSignals(true);
    if(state==AntiPhase){
        invertButton->setChecked(true);
        invertButton->setText(tr("Inverted"));
        invertButton->setStyleSheet("color: rgb(0, 0, 255)");
    }
    else if(state==InPhase){
        invertButton->setChecked(false);
        invertButton->setText(tr("Invert"));
        invertButton->setStyleSheet("color: rgb(0, 0, 0)");
    }
    if(blocked)
        invertButton->blockSignals(false);
}

double DACGain::getGain(){
    return knob->Value();
}

void DACGain::slot_muteButton_Clicked(bool state){
    main_tab->setMode(USER);
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
    main_tab->setMode(USER);
    datagram.clear();
    datagram[0]= INVERT_DAC_CHANNEL;
    datagram[1]= state;
    datagram[2]=channelID;
    WRITEDATAGRAM

}

 void DACGain::slot_gainChanged(double gain){
     main_tab->setMode(USER);
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
    UDP_Socket =    udp->UDP_Socket;
    IP_XMOS =       udp->IP_XMOS;
    port_XMOS =     udp->port_XMOS;
    main_tab =  this->parent()->findChild<MainTab*>("MainTab");
    QString aliasText[8] ={"Left Top" , "Right Top" ,"Left Sub" , "Right Sub"  , "Center Top" , "Rear" , "Center Sub" , "Center Sub" };

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
    groupBox->setFixedHeight(250);
    groupBox->setFixedWidth(800);
    topLayout->addWidget(groupBox,2,0,1,4);
    groupBoxReconstruct = new QGroupBox;
    groupBoxReconstruct->setTitle(tr("Reconstruction filter"));
    groupBoxReconstruct->setToolTip(tr("The settings of the reconstruction filter of the DAC\nFeature TBD!!"));


    IIRlabel = new QLabel;
    IIRlabel->setText("IIR filter for DSD");
    IIRFilter = new QComboBox;
    IIRFilter->addItem("Normal",QVariant(0));
    IIRFilter->addItem("f = 50kHz",QVariant(1));
    IIRFilter->addItem("f = 60kHz",QVariant(2));
    IIRFilter->addItem("f = 70kHz",QVariant(3));
    IIRFilter->setToolTip("IIR Bandwith of reconstruction filter in DSD mode");
    IIRFilter->setMaximumWidth(100);
    IIRFilter->setEnabled(false);
    //connect(IIRFilter , SIGNAL(currentIndexChanged) , this , SLOT(slot_Filterchanged()));

    FIRlabel = new QLabel;
    FIRlabel->setText("FIR filter for PCM");
    FIRFilter = new QComboBox;
    FIRFilter->addItem("Fast Rolloff default"    , QVariant(FastRolloff_ES9018));
    FIRFilter->addItem("Slow Rolloff default"    , QVariant(SlowRolloff_ES9018));
    FIRFilter->addItem("Minimium phase 96kHz"   , QVariant(Minphase96k));
    FIRFilter->addItem("Slow Rolloff 96kHz"     , QVariant(Slow96k));
    FIRFilter->addItem("Minimium phase 192kHz"  , QVariant(Minphase192k));
    FIRFilter->addItem("Slow Rolloff 192kHz"    , QVariant(Slow192k));
    FIRFilter->addItem("Variable Fast Rolloff"  , QVariant(6));


    FIRFilter->setToolTip("FIR rolloff of reconstruction filter in PCM mode");
    FIRFilter->setMaximumWidth(130);
    //FIRFilter->setEnabled(false);
    FIRKnob = new Knob(this);
    FIRKnob->setRange(1 , 6 , 5);
    FIRKnob->setSingleStep(1);
    FIRKnob->setKnobColor("rgb(255, 255, 128)");
    FIRKnob->setDecimals(0);
    FIRKnob->setMaximumHeight(130);
    FIRKnob->setToolTip("Filter index for variable Fast Rolloff filter");
    FIRKnob->setDisabled(true);
    updateStopFreq();

    connect(FIRFilter , SIGNAL(currentIndexChanged(int)) , this , SLOT(slot_Filterchanged(int) ));
    connect(FIRKnob  , SIGNAL(valueChanged(double)) , this , SLOT(slot_FilterFreqChanged(double) ));

    layoutReconstruct = new QVBoxLayout;
    layoutReconstruct->addWidget(IIRlabel);
    layoutReconstruct->addWidget(IIRFilter);
    layoutReconstruct->addWidget(FIRlabel);
    layoutReconstruct->addWidget(FIRFilter);
    layoutReconstruct->addWidget(FIRKnob);
    groupBoxReconstruct->setFixedHeight(300);
    groupBoxReconstruct->setFixedWidth(150);
    groupBoxReconstruct->setLayout(layoutReconstruct);


    groupBoxDPLL = new QGroupBox;
    groupBoxDPLL->setTitle(tr("DPLL"));
    groupBoxDPLL->setToolTip(tr("The Digital Phase Locked Loop settings of the jitter eliminator"));

    DPLLlabel = new QLabel;
    DPLLlabel->setText("DPPL bandwith");
    DPLLbox = new QComboBox;
    DPLLbox-> addItem("No BW"   , QVariant(NO_BW));
    DPLLbox-> addItem("Lowest"  , QVariant(LOWEST_BW));
    DPLLbox-> addItem("Low"     , QVariant(LOW_BW));
    DPLLbox-> addItem("Mid-Low" , QVariant(MIDLOW_BW));
    DPLLbox-> addItem("Mid"     , QVariant(MID_BW));
    DPLLbox-> addItem("Mid-High", QVariant(MIDHIGH_BW));
    DPLLbox-> addItem("High"    , QVariant(HIGH_BW));
    DPLLbox-> addItem("Highest" , QVariant(HIGHEST_BW));
    DPLLbox-> addItem("Auto" ,    QVariant(AUTO));
    DPLLbox->setMaximumWidth(100);
    DPLLbox-> setCurrentIndex(AUTO);
    connect(DPLLbox , SIGNAL(currentIndexChanged(int)) , this , SLOT(slot_DPLLchanged(int)));

    DPLL_X128 = new QCheckBox;
    DPLL_X128->setToolTip("Multiply the DPPL settings with 128");
    DPLL_X128->setText("128X");
    connect(DPLL_X128 , SIGNAL(clicked(bool)) , this , SLOT(slot_DPPL_x128changed(bool)));

    //Button Lock
    buttonLock = new QPushButton(tr("NO LOCK"));
    buttonLock ->setFixedWidth(60);
    buttonLock ->setCheckable(false);
    buttonLock ->setToolTip(tr("The status of the DAC's digital phase locked loop"));
    //buttonLock ->setFlat(true);
    buttonLock ->setStyleSheet("color: rgb(255, 0, 0)");
    QFont font;
    font.setBold(true);
    buttonLock->setFont(font);

    layoutDPLL = new QVBoxLayout;
    layoutDPLL->addWidget(DPLLlabel);
    layoutDPLL->addWidget(DPLLbox);
    layoutDPLL->addWidget(DPLL_X128);
    layoutDPLL->addWidget(buttonLock);
    groupBoxDPLL->setFixedHeight(140);
    groupBoxDPLL->setFixedWidth(150);
    groupBoxDPLL->setLayout(layoutDPLL);


    topLayout->addWidget(groupBoxReconstruct,0,0,2,1);
    topLayout->addWidget(groupBoxDPLL ,0,1,1,1);

    groupDither = new QGroupBox(this);
    groupDither->setMaximumHeight(160);
    layoutDither = new QVBoxLayout;
    groupDither->setTitle(tr("Dither"));
    DitherKnob = new Knob(this);
    DitherKnob->setRange( -150 , -50 , 101);
    DitherKnob->setValue(-150);
    DitherKnob->setDecimals(0);
    DitherKnob->setKnobColor("rgb(255, 188, 225)");
    DitherKnob->setSingleStep(1);
    DitherKnob->setTitle("Dither [dBFS]");
    DitherKnob->setToolTip("Dither in dB relative Full Scale");
    layoutDither->addWidget(DitherKnob);
    groupDither->setLayout(layoutDither);
    topLayout->addWidget(groupDither ,1,1,1,1);
    connect(DitherKnob , SIGNAL(valueChanged(double)) , this , SLOT(slot_DitherChanged(double)));

    setLayout(topLayout);
}

void DACTab::setDither(unsigned char dB){
    DitherKnob->blockSignals(true);
    DitherKnob->setValue(double(-dB));
    DitherKnob->blockSignals(false);
}

void DACTab::slot_DitherChanged(double val){
    datagram.clear();
    datagram[0]=DITHER;
    datagram[1]=(char) (-val);
    WRITEDATAGRAM;
}

void DACTab::updateStopFreq(){
    if (FIRFilter->currentIndex() == FastRolloff_var){
     int i = ((int)FIRKnob->Value())-1;
     FIRKnob->setTitle(QString("fpass=%1Hz\nfstop=%2Hz").arg( (int)round(pass[i] * fs)).arg( (int)round(stop[i] * fs) ));
    }else
     FIRKnob->setTitle(QString("fpass=\nfstop="));
}

void DACTab::slot_Filterchanged(int type){
    main_tab->setMode(USER);
    updateStopFreq();
    datagram.clear();
    datagram[0]=DAC_RECONSTRUCTION_FILTER;
    datagram[2]=(char) IIRFilter->currentIndex();
    if(type == FastRolloff_var){
         FIRKnob->setDisabled(false);
         datagram[1]=(char) (FastRolloff_var + FIRKnob->Value());
         updateStopFreq();
    }else{
         FIRKnob->setDisabled(true);
         datagram[1]=(char) type;
         }
    WRITEDATAGRAM;

}

void DACTab::slot_FilterFreqChanged(double i){
    updateStopFreq();
    datagram.clear();
    datagram[0]=DAC_RECONSTRUCTION_FILTER;
    datagram[1]=(char) (FastRolloff_var + char(i));
    datagram[2]=(char) IIRFilter->currentIndex();
    WRITEDATAGRAM;
    datagram.clear();
}

void DACTab::setFIRFilter(int settings){
    if(settings >=FastRolloff_var){
        FIRFilter -> setCurrentIndex(FastRolloff_var);
        FIRKnob -> setValue(settings - FastRolloff_var +1);
    }else
        FIRFilter->setCurrentIndex(settings);
}

void DACTab::setDPLL_BW(enum DPLL_BW_t settings){
    //QDebug(settings);
    DPLLbox->setCurrentIndex((int) settings);
}

void DACTab::setDPLL_128X(bool state){
    DPLL_X128->setChecked(state);
}

void DACTab::slot_sendDACsettings(){
    DAC_settings_t* DAC=new DAC_settings_t;
    const char *ptr = (const char *) DAC;

    for(int ch=0; ch<CHANNELS ; ch++){
        DAC->channel[ch].Gain = (unsigned char) round(-2*channel[ch]->getGain());
        DAC->channel[ch].mute = channel[ch]->getMute();
        DAC->channel[ch].polarity = channel[ch]->getPolarity();
        DAC->channel[ch].MasterTrim=-1;
        }
    DAC->FIRrolloff = (FIRfilter_t)FIRFilter->currentIndex();
    if(DAC->FIRrolloff==6)
        DAC->FIRrolloff = 6 + (int)FIRKnob->Value();
    DAC->IIRfilter = (IIRBandWidth_t) IIRFilter->currentIndex();
    DAC->DPPL_BW = (DPLL_BW_t) DPLLbox->currentIndex();
    DAC->DPLL_BWx128 = (bool_t) DPLL_X128->isChecked();
    DAC->DitherdB = (unsigned char) (-DitherKnob->Value());
   //main_tab = parent()->findChild("MainTab");
    DAC->MasterVolume = (unsigned char) -2*(main_tab->getMasterVolume()); //½dB attenuation steps
    if(main_tab->getMuteState())
        DAC->MuteAll = True;
    else
        DAC->MuteAll = False;
    datagram.clear();
    datagram[0]=SET_DACsettings;
    datagram.insert(4 , ptr , sizeof(DAC_settings_t));
    WRITEDATAGRAM

}
void DACTab::slot_DPLLchanged(int val){
    main_tab->setMode(USER);
    datagram.clear();
    datagram[0]=DAC_DPPL;
    datagram[1]=(char) val;
    WRITEDATAGRAM
}

void DACTab::slot_DPPL_x128changed(bool val){
    main_tab->setMode(USER);
    datagram.clear();
    datagram[0]=DAC_DPPL_128X;
    datagram[1]=(char) val;
    WRITEDATAGRAM
}

void DACTab::setLockAndFs(bool state, int new_fs){
    fs = new_fs;
    if(state){
        buttonLock ->setStyleSheet("color: rgb(0, 255, 0)");
        buttonLock ->setText(tr("LOCKED"));
        updateStopFreq();

    }else{
        buttonLock ->setStyleSheet("color: rgb(255, 0, 0)");
        buttonLock ->setText(tr("NO LOCK"));

    }
}
