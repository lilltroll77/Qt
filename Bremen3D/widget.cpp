#include "widget.h"
#include "QDebug"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //Setup network

    statusbar= this->parent()->findChild<QStatusBar*>("statusbar");
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
    main_tab->setObjectName("MainTab");
    mixer_tab   = new MixerTab(this, &udp);
    mixer_tab->setObjectName("MixerTab");
    eq_tab      = new EQTab(this    , &udp);
    eq_tab->setObjectName("EqTab");
    //adc_tab     = new ADCTab(this   ,&udp);
    dac_tab     = new DACTab(this , &udp);
    dac_tab->setObjectName("DACTab");

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
    unsigned char val;
    int ch;
    int* fs;
    float* LinkedFc;
    float* PreGain_f;
    int Format_status;
    int Channel_status;
    int Error_status;
    qint64 size;
    port = new quint16(XMOS_PORT) ;
    IP_XMOS= new QHostAddress(XMOS_IPADRESS);

    size=UDP_Socket->readDatagram(datagram_RX , DATAGRAM_RX_MAXLEN ,IP_XMOS, port);
    if(size>0){
        /** Check att IP_RX == IP_XMOS   **/

        switch((int) datagram_RX[0]){
        case MASTERVOLUME_CHANGED:
            val = (unsigned char) datagram_RX[1];
            main_tab->setMasterVolume( ((int)val)/-2 , true);
            //qDebug()<<val;
            break;
        case SPDIF_CHANGED:

            Channel_status = int(datagram_RX[1]);
            Format_status = int(datagram_RX[2]);
            Error_status = int(datagram_RX[3]);
            //qDebug() << "SPDIF: " << Format_status << "," <<Channel_status << "," << Error_status;
            if(Format_status>>6)
                main_tab->setFormat(PCM);
            else if(Format_status>>5)
                main_tab->setFormat(IEC61937);
            else if(Format_status>>4)
                main_tab->setFormat(DTS_LD);
            else if(Format_status>>3)
                main_tab->setFormat(DTS_CD);

            main_tab->setPro(       Channel_status>>3 &1);
            main_tab->setCopy(      Channel_status>>2 &1);
            main_tab->setOriginal(  Channel_status>>1 &1);
            main_tab->setEmphasis(  Channel_status    &1);

            main_tab->setPLL(       Error_status>>4 &1);
            main_tab->setValidity(  Error_status>>3 &1);

            break;
        case INPUT_SOURCE:
            //qDebug()<<datagram_RX[1];
            main_tab->setInputSelector(datagram_RX[1] ,true);
            break;
        case PROGRAM_CHANGED:
            //qDebug()<<"Program changed";
            main_tab->setProgram(datagram_RX[1] , true);
            break;
        case DACLOCK_CHANGED:
            val = (unsigned char) datagram_RX[1];
            fs  = (int*) &datagram_RX[4];
            main_tab->setLock((bool) val , *fs);
            dac_tab->setLockAndFs((bool) val , *fs);
            eq_tab->fsChanged(*fs);
            break;
        case PING:
             statusbar->showMessage(tr("Connected"),1100);

           break;
        case GET_DACsettings:
            //qDebug()<<"Recieved DAC settings";
            DAC = (DAC_settings_t *) &datagram_RX[4];
            for(ch=0 ; ch< CHANNELS ; ch++){
                double gain =  -(double)DAC->channel[ch].Gain /2;
                dac_tab->channel[ch]->setGain(gain , true );
                dac_tab->channel[ch]->setMute( DAC->channel[ch].mute ,true);
                dac_tab->channel[ch]->setPolarity( DAC->channel[ch].polarity , true);
            }
                dac_tab->setDPLL_BW( DAC->DPPL_BW);
                dac_tab->setDPLL_128X((bool) DAC->DPLL_BWx128);
                dac_tab->setFIRFilter( DAC->FIRrolloff );
                dac_tab->setDither( DAC->DitherdB);
                main_tab->setMasterVolume(((int)DAC->MasterVolume)/-2 , true);
                main_tab->setMuteState( DAC->MuteAll , true);
            break;
        case GET_EQsettings:
            //qDebug()<<"Recieved EQ settings";
            ch=datagram_RX[4];
            EQ= (EQ_channel_t*) &datagram_RX[4*sizeof(int)];
            eq_tab-> channel[ch]->setDelay((double)EQ->delay/1000 , true );
            for(int sec=0 ; sec<SECTIONS ; sec++){
                eq_tab-> channel[ch]->eqSection[sec]->setFilterActive(EQ->section[sec].active , true);
                eq_tab-> channel[ch]->eqSection[sec]->setFilterType(EQ->section[sec].type , true);
                eq_tab-> channel[ch]->eqSection[sec]->setFc(EQ->section[sec].Fc , true);
                eq_tab-> channel[ch]->eqSection[sec]->setLinked(EQ->section[sec].link , true);
                eq_tab-> channel[ch]->eqSection[sec]->setQ(EQ->section[sec].Q , true);
                eq_tab-> channel[ch]->eqSection[sec]->setGain(EQ->section[sec].Gain , true);

            }
            if(ch==CHANNELS-1){
                LinkedFc  = (float*) &datagram_RX[2*sizeof(int)];
                PreGain_f = (float*) &datagram_RX[3*sizeof(int)];
                //PreGain_d= (double) *PreGain_f;
                eq_tab->setPreGain(*PreGain_f , true);
                eq_tab->setLinkedFc( *LinkedFc , true); //Must be done in the end
                for(ch=0 ; ch<CHANNELS ; ch++){
                    for(int sec=0 ; sec<SECTIONS ; sec++)
                        eq_tab->channel[ch]->eqSection[sec]->updateSettingsAndPlot(false , eq_tab->fs); //update all freqz for all sections and for all channels
                    eq_tab->channel[ch]->recalc_graph(); //update the plot for that channel
                }
            }


           break;
        case GET_MIXERsettings:
            //qDebug()<<"Recieved Mixer settings";
            const double C=2147483647;
            int program = (double) datagram_RX[sizeof(int)];
            main_tab->setProgram(program, true);
            XMOSMixer = (XMOSMixer_settings_t*) &datagram_RX[2*sizeof(int)];
            double p = (double) XMOSMixer->P /C;
            double k = (double) XMOSMixer->K /C;
            mixer_tab->setP(p , true);
            mixer_tab->setK(k , true);
            mixer_tab->setK_type(XMOSMixer->k_choice , true);
            break;
        /*default:
            qDebug()<<"Unknown UDP command";
            break;*/
        }
    }
}

int Widget::getCurrentTabIndex(){
    return tabWidget->currentIndex();
}



