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
    box_input = new QGroupBox;
    input_selector = new QComboBox;
    box_layout = new QVBoxLayout;
    input_selector->addItem(tr("TOSLINK"),QVariant(0));
    input_selector->addItem(tr("AES/EBU"),QVariant(1));
    input_selector->addItem(tr("ANALOG"),QVariant(2));
    input_selector->setContentsMargins(5,5,5,5);
    input_selector->setMaximumWidth(75);
    box_layout->addWidget(input_selector);
    box_input->setToolTip(tr("Input selector"));
    box_input->setTitle(tr("Input selector"));
    box_input->setLayout(box_layout);
    box_input->setMaximumWidth(150);
    box_input->setMaximumHeight(50);

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
    box_program = new QGroupBox;
    box_program->setToolTip(tr("Program selector"));
    box_program->setTitle(tr("Program selector"));
    box_program->setMaximumWidth(350);
    box_program->setMaximumHeight(200);
    programLayout = new QGridLayout;
    for(int i=0; i<4 ;i++){
        radiobuttons[i] = new QRadioButton(QString("Program %1").arg(i));
        radiobuttons[i]->setFixedWidth(100);programLayout->addWidget(radiobuttons[i],i,1);
        signal_mapper->setMapping(radiobuttons[i] , i);
//SIGNALMAPPER
        connect(radiobuttons[i] , SIGNAL(clicked(bool)) , signal_mapper , SLOT(map()) );
   }
    box_program->setLayout(programLayout);


    //Mixer Box
    knob_bremen3D = new Knob(this , linScale);
    box_mixer = new QGroupBox;
    mixerLayout = new QVBoxLayout;
    knob_bremen3D-> setTitle("Bremen 3D [%]");
    knob_bremen3D-> setKnobColor("rgb(255, 127, 255)");
    knob_bremen3D->setRange(0,200,40);
    knob_bremen3D->setDecimals(0);
    knob_bremen3D->setSingleStep(1);
    knob_bremen3D->setValue(0);
    mixerLayout->addWidget(knob_bremen3D);
    box_mixer->setTitle(tr("Mixer"));
    box_mixer->setToolTip(tr("Channel mixer settings"));
    box_mixer->setLayout(mixerLayout);
    box_mixer->setMaximumHeight(200);
    box_mixer->setMaximumWidth(120);

    // Top layout
    top_layout->addWidget(box_input,0,1);
    top_layout->addWidget(box_program,0,0);
    top_layout->addWidget(box_ethernet,1,0);
    top_layout->addWidget(box_mixer,1,1);
    top_layout->addWidget(box_volume,0,2 ,2,1);

    //SIGNAL
    connect(slider_MasterVolume ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumesliderChanged(int) ) );
    connect(spinbox_volume      ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumeSpinboxChanged(int)) );
    connect(buttonMute ,            SIGNAL(toggled(bool))           , this ,    SLOT(slot_mutebuttonToggled(bool)) );
    connect(knob_bremen3D ,         SIGNAL(valueChanged(double))    , this ,    SLOT(slot_effectChanged(double))  );
    connect(buttonPingXMOS ,        SIGNAL(clicked())               , this ,    SLOT(slot_pingXMOS() ) );
    connect(buttonSyncFromXMOS ,    SIGNAL(clicked())               , this ,    SLOT(slot_syncFromXMOS()));
    connect(buttonSyncToXMOS ,      SIGNAL(clicked())               , this ,    SLOT(slot_synctoXMOS()));
    connect(input_selector ,        SIGNAL(currentIndexChanged(int)), this ,    SLOT( slot_inputselectorChanged(int)) );
    //connect(signal_mapper,          SIGNAL(mapped(int))             , this->parent()->parent() , SLOT(programChanged(int)) );
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
        datagram.clear();
        datagram.append("MUTE ALL");
    }
    else{
        font.setBold(false);
        buttonMute->setText(tr("MUTE"));
        buttonMute->setFont(font);
        buttonMute->setStyleSheet("color: rgb(0, 0, 0)");
        datagram.clear();
        datagram.append("UNMUTE ALL");
    }
   WRITEDATAGRAM
}

void MainTab::slot_volumesliderChanged(int value){
    spinbox_volume->setValue(value);
    datagram.clear();
    datagram.append(QString("Volume changed to %1").arg(value));
    }

void MainTab::slot_volumeSpinboxChanged(int value){
    slider_MasterVolume->setValue(value);
    datagram.clear();
    datagram.append(QString("Volume changed to %1").arg(value));
    WRITEDATAGRAM
}

void MainTab::slot_effectChanged(double value){
    datagram.clear();
    datagram.append(QString("Bremen3D changed to %1 %").arg(value));
    WRITEDATAGRAM
}

void MainTab::slot_programChanged(int index){
     datagram.clear();
     datagram.append(QString("PROGRAM %1 SELECTED").arg(index));
     WRITEDATAGRAM
}

void MainTab::slot_pingXMOS(){
    datagram.clear();
    datagram.append("PING");
    WRITEDATAGRAM
}
void MainTab::slot_syncFromXMOS(){
    datagram.clear();
    datagram.append("SYNC FROM XMOS");
    WRITEDATAGRAM
}

void MainTab::slot_synctoXMOS(){
    datagram.clear();
    datagram.append("SYNC TO XMOS");
    WRITEDATAGRAM
}

void MainTab::slot_inputselectorChanged(int index){
    datagram.clear();
    datagram.append(QString("INPUT changed to %1").arg(index) );
    WRITEDATAGRAM
}
