#include "widget.h"
#include "QDebug"

Widget::Widget(QWidget *parent , QStatusBar *statusbar)
    : QWidget(parent)
{
    //Setup network

    UDP_Socket = new QUdpSocket;
    UDP_Socket ->setObjectName("UDP Socket");
    IP_XMOS = new QHostAddress(XMOS_IPADRESS);
    IP_LOCAL = new QHostAddress(LOCAL_IPADRESS);

    udp.UDP_Socket = UDP_Socket ;
    udp.IP_XMOS = IP_XMOS;
    udp.IP_LOCAL = IP_LOCAL;

    //UDP_Socket->bind( *IP_LOCAL ,XMOS_PORT );
    connect(UDP_Socket , SIGNAL(readyRead()),this, SLOT(readDatagram()));

    UDP_Socket->writeDatagram("HELLO", 5, *IP_XMOS,  XMOS_PORT);

    tabWidget   = new QTabWidget(this);
    main_tab    = new MainTab(this  , &udp);
    mixer_tab   = new MixerTab(this, &udp);
    eq_tab      = new EQTab(this    , &udp);
    //adc_tab     = new ADCTab(this   ,&udp);
    dac_tab     = new DACTab(this   , &udp);

    tabWidget-> addTab(main_tab, tr("Main"));
    tabWidget->addTab(mixer_tab, tr("Matrix mixer"));
    tabWidget-> addTab(eq_tab, tr("Filter && Delay"));
    //tabWidget-> addTab(adc_tab, tr("ADC"));
    tabWidget-> addTab(dac_tab, tr("DAC"));
    tabWidget-> setCurrentIndex(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

Widget::~Widget()
{

}

void Widget::readDatagram(){
    char i;
    int ch;
    qint64 size;
    port = new quint16(XMOS_PORT) ;
    IP_XMOS= new QHostAddress(XMOS_IPADRESS);

    size=UDP_Socket->readDatagram(datagram_RX , DATAGRAM_RX_MAXLEN ,IP_XMOS, port);
    if(size>0){
        /** Check att IP_RX == IP_XMOS   **/

        switch((int) datagram_RX[0]){
        case PROGRAM_CHANGED:
            i=datagram_RX[1];
            if(i<4){
                main_tab->radiobuttons[i]->setChecked(true); ;
            }
            //statusbar->showMessage(tr("Program changed") ,5000);

            break;
        case MASTERVOLUME:
            main_tab->slider_MasterVolume->setValue(- (int)datagram_RX[1]);
            break;
        case PING:
           break;
        case GET_DACsettings:
            DAC = (DAC_settings_t *) &datagram_RX[4];
            bool phase[8];
            bool state;
            for(ch=0 ; ch< CHANNELS ; ch++)
                phase[ch]= (DAC->Polarity >> ch)&1;
            for(ch=0 ; ch< CHANNELS ; ch++){
                dac_tab->channel[ch]->knob->setValue( -(double)DAC->channel[ch].Gain /2 );
                if(DAC->channel[ch].mute == True)
                    state = true;
                else
                    state=false;
                dac_tab->channel[ch]->muteButton->setChecked( state);
                dac_tab->channel[ch]->invertButton->setChecked(phase[ch]);
            }
            break;
        case GET_EQsettings:
            ch=datagram_RX[4];
            EQ= (EQ_channel_t*) &datagram_RX[8];
            eq_tab-> channel[ch]->knob_delay->setValue( (double)EQ->delay/1000 );
            for(int sec=0 ; sec<SECTIONS ; sec++){
                eq_tab-> channel[ch]->eqSection[sec]->groupBox->setChecked(EQ->section[sec].active);
                eq_tab-> channel[ch]->eqSection[sec]->filterType->setCurrentIndex(EQ->section[sec].type);
                eq_tab-> channel[ch]->eqSection[sec]->knob_fc->setValue(EQ->section[sec].Fc);
                eq_tab-> channel[ch]->eqSection[sec]->knob_Q->setValue(EQ->section[sec].Q);
                eq_tab-> channel[ch]->eqSection[sec]->knob_gain->setValue(EQ->section[sec].Gain);
            }
           break;
        case GET_MIXERsettings:
            Mixer = (Mixer_settings_t*) &datagram_RX[4];
           break;
        }
    }
}

int Widget::getCurrentTabIndex(){
    return tabWidget->currentIndex();
}



