#include "widget.h"


Widget::Widget(QWidget *parent , QStatusBar *statusbar)
    : QWidget(parent)
{

    UDP_Socket->bind( *IP_LOCAL , LOCAL_PORT);
    connect(UDP_Socket , SIGNAL(readyRead()),this, SLOT(readDatagram()));


    tabWidget   = new QTabWidget(this);
    main_tab    = new MainTab(this);
    eq_tab      = new EQTab(this); //defined global static
    adc_tab     = new ADCTab(this);
    dac_tab     = new DACTab(this);

    tabWidget-> addTab(main_tab, tr("Main"));
    tabWidget-> addTab(eq_tab, tr("Filter && Delay"));
    tabWidget-> addTab(adc_tab, tr("ADC"));
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
    size=UDP_Socket->readDatagram(datagram_RX , DATAGRAM_RX_MAXLEN ,IP_RX, port);
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

