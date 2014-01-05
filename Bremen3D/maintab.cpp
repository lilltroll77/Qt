#include "maintab.h"
#include "UDPcommands.h"

MainTab::MainTab(QWidget *parent) :
    QWidget(parent)
{
    toplevel_statusbar=statusbar;
    top_layout = new QGridLayout;
    signal_mapper = new QSignalMapper;

//Volume box
    layout_volume = new QVBoxLayout;
    box_volume = new QGroupBox;
    slider_MasterVolume = new QSlider;
    slider_MasterVolume->setToolTip(tr("Master Volume"));
    slider_MasterVolume->setRange(-100,0);
    slider_MasterVolume->setFixedHeight(220);
    slider_MasterVolume->setFixedWidth(60);
    slider_MasterVolume->setTickInterval(20);
    slider_MasterVolume->setTickPosition(QSlider::TicksBothSides);
//SIGNAL
    signal_mapper->setMapping(slider_MasterVolume , MASTERVOLUME);
    connect(slider_MasterVolume , SIGNAL(valueChanged(int)) , signal_mapper , SLOT(map()) );

//Mute button
    buttonMute = new QPushButton(tr("MUTE"));
    buttonMute ->setFixedWidth(60);
    buttonMute ->setCheckable(true);
//SIGNAL
    signal_mapper->setMapping(buttonMute ,MUTE_ALL);
    connect(buttonMute , SIGNAL(clicked()) , signal_mapper , SLOT(map()) );

    layout_volume ->addWidget(slider_MasterVolume);
    layout_volume ->addWidget(buttonMute);
    box_volume->setLayout(layout_volume);
    box_volume->setTitle(tr("Master Volume"));
    box_volume->setFixedSize(86,300);
    box_volume->setAlignment(Qt::AlignHCenter);


//Input selector box
    box_input = new QGroupBox;
    input_selector = new QComboBox;
    box_layout = new QVBoxLayout;
    input_selector->addItem(tr("TOSLINK"),QVariant(0));
    input_selector->addItem(tr("AES/EBU"),QVariant(1));
    input_selector->addItem(tr("ANALOG"),QVariant(2));
    input_selector->setContentsMargins(5,5,5,5);
    input_selector->setMaximumWidth(75);
//SINGAL
    signal_mapper->setMapping(input_selector , INPUT_SOURCE);
    connect(input_selector , SIGNAL(currentIndexChanged(int)) , signal_mapper , SLOT(map()) );

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
//SIGNAL
    signal_mapper->setMapping(buttonPingXMOS , PING );
    connect(buttonPingXMOS , SIGNAL(clicked()) , signal_mapper , SLOT(map()) );
    signal_mapper->setMapping(buttonSyncFromXMOS , SYNC_FROM_XMOS );
    connect(buttonSyncFromXMOS , SIGNAL(clicked()) , signal_mapper , SLOT(map()) );
    signal_mapper->setMapping(buttonSyncToXMOS , SYNC_TO_XMOS );
    connect(buttonSyncToXMOS , SIGNAL(clicked()) , signal_mapper , SLOT(map()) );


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
        radiobuttons[i]->setFixedWidth(100);

        openbuttons[i] = new QPushButton(tr("Open"));
        openbuttons[i]->setFixedWidth(40);

        savebuttons[i] = new QPushButton(tr("Save"));
        savebuttons[i]->setFixedWidth(40);
        programLayout->addWidget(radiobuttons[i],i,0);
        programLayout->addWidget(openbuttons[i],i,1);
        programLayout->addWidget(savebuttons[i],i,2);
//SIGNALS
        signal_mapper->setMapping(radiobuttons[i] , PROGRAM_CHANGED );
        connect(radiobuttons[i] , SIGNAL(clicked()) , signal_mapper , SLOT(map()) );
    }
    box_program->setLayout(programLayout);


 //Crossover box
    knob_crossover = new Knob(this ,logScale );
    box_crossover = new QGroupBox;
    crossoverLayout = new QVBoxLayout;
    knob_crossover-> setTitle("Fc [Hz]");
    knob_crossover-> setKnobColor("rgb(255, 127, 127)");
    knob_crossover->setRange(20,2500,100);
    knob_crossover->setDecimals(0);
    knob_crossover->setSingleStep(1);
    knob_crossover->setValue(80);
    crossoverLayout->addWidget(knob_crossover);
    box_crossover->setTitle(tr("Crossover"));
    box_crossover->setToolTip(tr("Crossover / linked center frequency"));
    box_crossover->setLayout(crossoverLayout);
    box_crossover->setMaximumHeight(200);
    box_crossover->setMaximumWidth(120);
    //SIGNALS
    signal_mapper->setMapping(knob_crossover , CROSSOVER_FREQ);
    connect(knob_crossover , SIGNAL(valueChanged(double)) , signal_mapper , SLOT(map()) );


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
//SIGNAL
    signal_mapper->setMapping(knob_bremen3D , MIXERVALUE );
    connect(knob_bremen3D , SIGNAL(valueChanged(double)) , signal_mapper , SLOT(map()) );

    // Top layout

    top_layout->addWidget(box_input,1,2,Qt::AlignBottom);
    top_layout->addWidget(box_program,0,0);
    top_layout->addWidget(box_ethernet,1,0);
    top_layout->addWidget(box_crossover,0,1);
    top_layout->addWidget(box_mixer,1,1);
    top_layout->addWidget(box_volume,0,2 ,2,1);

    // connect(reportFileButton, &QPushButton::clicked,signalMapper, &QSignalMapper::map);



    connect(signal_mapper, SIGNAL(mapped(int)) , this, SLOT(generateDatagram(int)) );
    setLayout(top_layout);

}



void MainTab::generateDatagram(int command){
    datagram.clear();
    switch(command){
    case MASTERVOLUME:
        datagram.append(QString("Volume: %1").arg(slider_MasterVolume->value()));
    break;
    case MUTE_ALL:
        datagram.append("MUTE ALL");
        break;
    case INPUT_SOURCE:
        datagram.append(QString("INPUT changed to %1").arg(input_selector->currentIndex()) );
        break;
    case PING:
        datagram.append("PING");
        break;
    case SYNC_FROM_XMOS:
        datagram.append("SYNC_FROM_XMOS");
        break;
    case SYNC_TO_XMOS:
        datagram.append("SYNC_TO_XMOS");
        break;
    case PROGRAM_CHANGED:
        for(int i=0 ;i<4 ;i++){
            if(radiobuttons[i]->isChecked()==true)
                datagram.append(QString("PROGRAM %1 SELECTED").arg(i));
        }

        break;
    case CROSSOVER_FREQ:
        datagram.append(QString("CROSSOVER frequency:  %1 ").arg(knob_crossover->Value() ) );
        break;
    case MIXERVALUE:
        datagram.append(QString("MIXER: %1").arg(knob_bremen3D->Value()) );
        break;
    }
     UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);

}
