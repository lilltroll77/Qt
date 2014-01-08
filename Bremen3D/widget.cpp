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
    eq_tab      = new EQTab(this    , &udp);
    //adc_tab     = new ADCTab(this   ,&udp);
    dac_tab     = new DACTab(this   , &udp);

    tabWidget-> addTab(main_tab, tr("Main"));
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
    qint64 size;
    quint16 *port;
    QHostAddress *IPaddress;
    size=UDP_Socket->readDatagram(datagram_RX , DATAGRAM_RX_MAXLEN ,IPaddress, port);
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
     }
    }
}

int Widget::getCurrentTabIndex(){
    return tabWidget->currentIndex();
}



