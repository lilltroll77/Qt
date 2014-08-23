#include "maintab.h"
#include <QDebug>



MainTab::MainTab(QWidget *parent ,  Network *udp) :
    QWidget(parent){

    UDP_Socket = udp->UDP_Socket;
    IP_XMOS = udp->IP_XMOS;
    port_XMOS = udp->port_XMOS;

    toplevel_statusbar=statusbar;
    top_layout = new QGridLayout;
    signal_mapper = new QSignalMapper;

//Volume box
    layout_volume = new QVBoxLayout;
    box_volume = new QGroupBox;
    spinbox_volume = new QSpinBox;
    spinbox_volume->setRange(-100,0);
    spinbox_volume->setFixedWidth(60);
    spinbox_volume->setAlignment(Qt::AlignCenter);
    slider_MasterVolume = new QSlider;
    slider_MasterVolume->setToolTip(tr("Master Volume"));
    slider_MasterVolume->setRange(-100,0);
    slider_MasterVolume->setFixedHeight(200);
    slider_MasterVolume->setFixedWidth(60);
    slider_MasterVolume->setTickInterval(10);
    slider_MasterVolume->setTickPosition(QSlider::TicksBothSides);

//Mute button
    buttonMute = new QPushButton(tr("MUTE"));
    buttonMute ->setFixedWidth(60);
    buttonMute ->setCheckable(true);

    layout_volume ->addWidget(slider_MasterVolume);
    layout_volume ->addWidget(spinbox_volume);
    layout_volume ->addWidget(buttonMute);
    box_volume->setLayout(layout_volume);
    box_volume->setTitle(tr("Master Volume"));
    box_volume->setFixedSize(86,350);
    box_volume->setAlignment(Qt::AlignHCenter);

//******************************************************************************
//Input selector box
    input_Groupbox = new QGroupBox;
    input_ComboBox = new QComboBox;
    input_layout = new QVBoxLayout;

    input_Groupbox->setToolTip(tr("Input selector"));
    input_Groupbox->setTitle(tr("Input selector"));
    input_Groupbox->setMaximumWidth(150);
    input_Groupbox->setMaximumHeight(50);

    input_ComboBox->addItem(tr("TOSLINK"),QVariant(0));
    input_ComboBox->addItem(tr("AES/EBU"),QVariant(1));
    input_ComboBox->setContentsMargins(5,5,5,5);
    input_ComboBox->setMaximumWidth(75);
    //input_selector->addItem(tr("ANALOG"),QVariant(2));
    input_layout->addWidget(input_ComboBox);
    input_Groupbox->setLayout(input_layout);

//Ethernet box
    box_ethernet        = new QGroupBox;
    buttonSyncFromXMOS  = new QPushButton(tr("SYNC XMOS->GUI"));
    buttonSyncToXMOS    = new QPushButton(tr("SYNC GUI->XMOS"));
    buttonPingXMOS      = new QPushButton(tr("PING XMOS"));
    ethernetLayout      = new QGridLayout;
    lineEditXMOSIP      = new QLineEdit;
    buttonSyncFromXMOS->setToolTip(tr("Sync GUI from XMOS settings"));
    buttonSyncToXMOS->setToolTip(tr("Sync XMOS from GUI settings"));
    buttonPingXMOS->setToolTip(tr("Test if XMOS is available on Network"));
    buttonSyncFromXMOS->setFixedWidth(100);
    buttonSyncToXMOS->setFixedWidth(100);
    buttonPingXMOS->setFixedWidth(100);

    lineEditXMOSIP->setToolTip("IP Adress of XMOS box");
    lineEditXMOSIP->setText(QString("IP: %1").arg(XMOS_IPADRESS));
    lineEditXMOSIP->setFixedWidth(120);
    lineEditXMOSIP->setInputMask("IP: 000.000.000.000");
    ethernetLayout->addWidget(buttonSyncFromXMOS,0,1);
    ethernetLayout->addWidget(buttonSyncToXMOS,1,1);
    ethernetLayout->addWidget(buttonPingXMOS,1,0);
    ethernetLayout->addWidget(lineEditXMOSIP,0,0);
    box_ethernet->setToolTip(tr("Ethernet settings"));
    box_ethernet->setTitle(tr("Ethernet"));
    box_ethernet->setMaximumWidth(350);
    box_ethernet->setMaximumHeight(200);
    box_ethernet->setLayout(ethernetLayout);

//Program selection box
    program_Groupbox    = new QGroupBox;
    program_ComboBox    = new QComboBox;
    program_layout      = new QVBoxLayout;

    program_Groupbox -> setToolTip(tr("Program selector"));
    program_Groupbox -> setTitle(tr("Program selector"));
    program_Groupbox -> setMaximumWidth(200);
    program_Groupbox -> setMaximumHeight(50);
    for(int i=1; i<16 ; i++)
       program_ComboBox->addItem(QString("Program %1").arg(i),QVariant(i));

    program_ComboBox->setContentsMargins(5,5,5,5);
    program_ComboBox->setMaximumWidth(125);

    program_layout->addWidget(program_ComboBox);
    program_Groupbox->setLayout(program_layout);

    // Top layout
    top_layout->addWidget(input_Groupbox    ,0,1);
    top_layout->addWidget(program_Groupbox  ,0,0);
    top_layout->addWidget(box_ethernet,1,0);
    //top_layout->addWidget(box_mixer,1,1);
    top_layout->addWidget(box_volume,0,2 ,2,1);

    //SIGNAL
    connect(slider_MasterVolume ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumesliderChanged(int) ) );
    connect(spinbox_volume      ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumeSpinboxChanged(int)) );
    connect(buttonMute ,            SIGNAL(toggled(bool))           , this ,    SLOT(slot_mutebuttonToggled(bool)) );
    //connect(knob_bremen3D ,         SIGNAL(valueChanged(double))    , this ,    SLOT(slot_effectChanged(double))  );
    connect(buttonPingXMOS ,        SIGNAL(clicked())               , this ,    SLOT(slot_pingXMOS() ) );
    connect(buttonSyncFromXMOS ,    SIGNAL(clicked())               , this ,    SLOT(slot_syncFromXMOS()));
    connect(buttonSyncToXMOS ,      SIGNAL(clicked())               , this ,    SLOT(slot_synctoXMOS()));
    connect(input_ComboBox ,        SIGNAL(currentIndexChanged(int)), this ,    SLOT( slot_inputselectorChanged(int)) );
    connect(program_ComboBox,       SIGNAL(currentIndexChanged(int)), this->parent()->parent() , SLOT(programChanged(int)) );
    connect(signal_mapper,          SIGNAL(mapped(int))             , this ,    SLOT(slot_programChanged(int)));


    setLayout(top_layout);

}

void MainTab::slot_mutebuttonToggled(bool state){
    QFont font;
    if(state){
        font.setBold(true);
        buttonMute->setText(tr("MUTED"));
        buttonMute->setFont(font);
        buttonMute->setStyleSheet("color: rgb(255, 0, 0)");
     }
    else{
        font.setBold(false);
        buttonMute->setText(tr("MUTE"));
        buttonMute->setFont(font);
        buttonMute->setStyleSheet("color: rgb(0, 0, 0)");
   }
    datagram.clear();
    datagram[0]= MUTE_ALL;
    datagram[1]=state;
    WRITEDATAGRAM
}

void MainTab::slot_volumesliderChanged(int value){
    spinbox_volume->blockSignals(true);
    spinbox_volume->setValue(value);
    spinbox_volume->blockSignals(false);
    datagram.clear();
    datagram[0]= MASTERVOLUME;
    datagram[1]=(uchar) -value;
    WRITEDATAGRAM
    }

void MainTab::slot_volumeSpinboxChanged(int value){
    slider_MasterVolume->blockSignals(true);
    slider_MasterVolume->setValue(value);
    slider_MasterVolume->blockSignals(false);
    datagram.clear();
    datagram[0]= MASTERVOLUME;
    datagram[1]=(uchar) -value;
    WRITEDATAGRAM
}

/*void MainTab::slot_effectChanged(double value){
    datagram.clear();
    datagram[0]=EFFECT;
    datagram[1]=(uchar) value;
    WRITEDATAGRAM
}*/

void MainTab::slot_programChanged(int index){
    datagram[0]=PROGRAM_CHANGED;
    datagram[1]=(uchar) index;
    WRITEDATAGRAM
}

void MainTab::slot_pingXMOS(){
    datagram.clear();
    datagram[0]=PING;
    WRITEDATAGRAM
}
void MainTab::slot_syncFromXMOS(){
    datagram.clear();
    datagram[0]=SYNC_FROM_XMOS;
    WRITEDATAGRAM
}

void MainTab::slot_synctoXMOS(){
    datagram.clear();
    ///TODO SEND ALL
    datagram[0] = SYNC_TO_XMOS;
    WRITEDATAGRAM
}

void MainTab::slot_inputselectorChanged(int index){
    datagram.clear();
    datagram[0] = INPUT_SOURCE;
    datagram[1] = (uchar) index;
    WRITEDATAGRAM
}
