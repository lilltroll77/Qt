#include "maintab.h"
#include "centralwidget.h"
#include <QDebug>

MainTab::MainTab(QWidget *parent ,  Network *udp_ref) :
    QWidget(parent){

    //udp = udp_ref;
    UDP_Socket = udp_ref->UDP_Socket;
    IP_XMOS = udp_ref->IP_XMOS;
    port_XMOS = udp_ref->port_XMOS;

    toplevel_statusbar=statusbar;
    top_layout = new QGridLayout;
    signal_mapper = new QSignalMapper;

//Program box

    program_Groupbox = new QGroupBox(this);
    program_knob = new Knob(this , linScale);
    program_layout = new QVBoxLayout(this);
    //button_preset = new QPushButton(tr("PRESET"));
    //button_preset->setMaximumWidth(60);
    //button_preset->setCheckable(false);
    program_Groupbox->setTitle("Current Mode: Preset");
    program_Groupbox->setObjectName("program_selector");

    //program_Groupbox->setUserData(0, userData );
    program_Groupbox->setFixedWidth(200);
    program_Groupbox->setFixedHeight(200);
    program_Groupbox->setToolTip(tr("16 different presets with EQ & Mixer settings\n(The settings is stored in flash in HW)"));
    program_knob->setRange(1 , 15 ,14);
    program_knob->setValue(1);
    program_knob->setDecimals(0);
    program_knob->setMinimumWidth(50);
    program_knob->setTitle("Program");
    program_layout->addWidget(program_knob);
    //program_layout->addWidget(button_preset);
    program_Groupbox->setLayout(program_layout);


//Volume box
    layout_volume = new QVBoxLayout;
    box_volume = new QGroupBox;
    box_volume->setTitle("Output");
    box_volume->setFixedWidth(150);
    spinbox_volume = new QSpinBox;
    spinbox_volume->setRange(-100,0);
    spinbox_volume->setValue(-50);
    spinbox_volume->setFixedWidth(60);
    spinbox_volume->setAlignment(Qt::AlignCenter);
    slider_MasterVolume = new QSlider;
    slider_MasterVolume->setToolTip(tr("Master Volume"));
    slider_MasterVolume->setRange(-100,0);
    slider_MasterVolume->setValue(-50);
    slider_MasterVolume->setFixedHeight(200);
    slider_MasterVolume->setFixedWidth(60);
    slider_MasterVolume->setTickInterval(10);
    slider_MasterVolume->setTickPosition(QSlider::TicksBothSides);

//Mute button
    buttonMute = new QPushButton(tr("MUTE"));
    buttonMute ->setFixedWidth(60);
    buttonMute ->setCheckable(true);

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

    labelFS = new QLabel("fs=0Hz");
    labelFS->setFixedWidth(60);

    layout_volume ->addWidget(slider_MasterVolume);
    layout_volume ->addWidget(spinbox_volume);
    layout_volume ->addWidget(buttonMute);
    layout_volume ->addWidget(buttonLock);
    layout_volume ->addWidget(labelFS);
    box_volume->setLayout(layout_volume);
    box_volume->setTitle(tr("Output"));
    box_volume->setFixedSize(86,350);
    box_volume->setAlignment(Qt::AlignHCenter);

//******************************************************************************
//Input selector box
    input_Groupbox = new QGroupBox(this);
    input_ComboBox = new QComboBox(this);
    input_layout = new QVBoxLayout(this);
    input_format = new QLabel(tr("Format") , this);
    input_copy = new QLabel(tr("Copyright") , this);
    input_orig = new QLabel(tr("Original") , this);
    input_emph= new QLabel(tr("Pre-emphasis") , this);
    input_pro = new QLabel(tr("Pro") , this);
    input_PLL = new QLabel(tr("PLL"), this);
    input_validity = new QLabel(tr("Validity") , this);

    input_Groupbox->setToolTip(tr("Input"));
    input_Groupbox->setTitle(tr("Input"));
    input_Groupbox->setFixedWidth(150);
    input_Groupbox->setMinimumHeight(100);

    input_ComboBox->addItem(tr("AES/EBU"),QVariant(0));
    input_ComboBox->addItem(tr("TOSLINK"),QVariant(1));
    input_ComboBox->addItem(tr("USB AUDIO"),QVariant(2));
    input_ComboBox->setCurrentIndex(1);
    input_ComboBox->setContentsMargins(5,5,5,5);
    input_ComboBox->setMaximumWidth(100);
    //input_selector->addItem(tr("ANALOG"),QVariant(2));
    input_layout->addWidget(input_ComboBox);
    input_layout->addWidget(input_format);
    input_layout->addWidget(input_copy);
    input_layout->addWidget(input_orig);
    input_layout->addWidget(input_emph);
    input_layout->addWidget(input_pro);
    input_layout->addWidget(input_validity);
    input_layout->addWidget(input_PLL);

    input_Groupbox->setLayout(input_layout);



    // Top layout
    top_layout->setColumnMinimumWidth(0,50);
    top_layout->addWidget(input_Groupbox ,3 , 1);
    top_layout->setColumnMinimumWidth(2,50);
    top_layout->addWidget(program_Groupbox ,3 , 3);
    top_layout->setColumnMinimumWidth(4,50);
    top_layout->addWidget(box_volume,0,5 ,9,1);
    //top_layout->setColumnStretch(4,1);
    top_layout->setColumnStretch(6,1);


    //SIGNAL
    connect(slider_MasterVolume ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumesliderChanged(int) ) );
    connect(spinbox_volume      ,   SIGNAL(valueChanged(int))       , this ,    SLOT(slot_volumeSpinboxChanged(int)) );
    connect(buttonMute ,            SIGNAL(toggled(bool))           , this ,    SLOT(slot_mutebuttonToggled(bool)) );
    connect(input_ComboBox ,        SIGNAL(currentIndexChanged(int)), this ,    SLOT(slot_inputselectorChanged(int)) );
    connect(program_knob ,          SIGNAL(valueChanged(double))    , this ,    SLOT(slot_programChanged(double)));

    setLayout(top_layout);

}

void MainTab::setValidity(int v){
    if(v==0){
        input_validity->setText("Valid data");
        input_validity->setToolTip(tr("Data is valid and is normally linear coded PCM audio"));
    }else{
        input_validity->setText("Invalid data");
        input_validity->setToolTip(tr("Data is invalid, or may be valid compressed audio"));
    }
}

void MainTab::setPLL(int pll){
    if(pll==0){
        input_PLL->setText("PLL locked");
        input_PLL->setToolTip(tr("PLL locked"));
    }else{
        input_PLL->setText("PLL unlocked");
        input_PLL->setToolTip(tr("PLL out of lock."));
    }
}

void MainTab::setPro(int pro){
    if(pro == 0){
        input_pro->setText("Consumer format");
        input_pro->setToolTip(tr("Received channel status block is in the consumer format"));
    }else{
        input_pro->setText("Professional format.");
        input_pro->setToolTip(tr("Received channel status block is in the professional format"));
    }
}

void MainTab::setCopy(int copy){
    if(copy == 1){
        input_copy->setText("Copyright: No");
        input_copy->setToolTip(tr("Copyright not asserted"));
    }else{
        input_copy->setText("Copyright: Yes");
        input_copy->setToolTip(tr("Copyright asserted"));
    }
}

void MainTab::setOriginal(int original){
    if(original == 0){
        input_orig->setText("Not original data");
        input_orig->setToolTip(tr("Received data is 1st generation or higher"));
    }else{
        input_orig->setText("Original data");
        input_orig->setToolTip(tr("Received data is original"));
    }
}

void MainTab::setEmphasis(int emph){
    if(emph == 1){
        input_emph->setText("Pre-emphasis: No");
        input_emph->setToolTip(tr("pre-emphasis not indicated"));
    }else{
        input_emph->setText("Pre-emphasis: Yes");
        input_emph->setToolTip(tr("50 μs/15 μs pre-emphasis indicated"));
    }
}



void MainTab::setFormat(enum format_t format){
    switch(format){
    case PCM:
         input_format->setText("Format: PCM");
         input_format->setToolTip(tr("Uncompressed PCM data"));
        break;
    case IEC61937:
        input_format->setText("Format: IEC61937");
        input_format->setToolTip(tr("IEC61937 data"));
        break;
    case DTS_LD:
        input_format->setText("Format: DTS LD");
        input_format->setToolTip(tr("DTS LD"));
        break;
    case DTS_CD:
        input_format->setText("Format: DTS CD");
        input_format->setToolTip(tr("DTS CD"));
        break;
    case DGTL_SIL:
        input_format->setText("Digital silent");
        input_format->setToolTip(tr("Digital Silence was detected"));
        break;
    }
}

void MainTab::setProgram(int new_program , bool blocked){
    if(blocked)
        program_knob->blockSignals(true);
    if(new_program==0)
        setMode(USER);
    else{
        program_knob->setValue((double)new_program);
        setMode(PRESET);
    }

    if(blocked)
        program_knob->blockSignals(false);
}

int MainTab::getProgram(){
    return (int) round(program_knob->Value());
}

void MainTab::setInputSelector(int val , bool blocked){
    if(blocked)
        input_ComboBox->blockSignals(true);
    input_ComboBox->setCurrentIndex(val);
    if(blocked)
        input_ComboBox->blockSignals(false);
}

int MainTab::getInputSelector(){
    return input_ComboBox->currentIndex();
}

void MainTab::sendSettings(){
    datagram.clear();
    datagram.append(SET_MAINsettings);
    datagram.append((char) getInputSelector());
    WRITEDATAGRAM
}

void MainTab::setLock(bool state , int new_fs){
    fs=new_fs;
    labelFS->setText(QString("fs=%1Hz").arg(fs));
    if(state){
        buttonLock ->setStyleSheet("color: rgb(0, 255, 0)");
        buttonLock ->setText(tr("LOCKED"));

    }else{
        buttonLock ->setStyleSheet("color: rgb(255, 0, 0)");
        buttonLock ->setText(tr("NO LOCK"));

    }
}

void MainTab::setMasterVolume(int val , bool blocked){
    if(blocked){
        slider_MasterVolume->blockSignals(true);
        spinbox_volume->blockSignals(true);
    }
    slider_MasterVolume->setValue(val);
    spinbox_volume->setValue(val);
    if(blocked)
        slider_MasterVolume->blockSignals(false);
        spinbox_volume->blockSignals(true);
    }

int MainTab::getMasterVolume(){
    return slider_MasterVolume->value();
}

void MainTab::setMuteState(bool state , bool blocked){
    if(blocked)
        buttonMute->blockSignals(true);
    buttonMute->setChecked(state);
    if(blocked)
        buttonMute->blockSignals(false);
}

bool MainTab::getMuteState(){
    return buttonMute->isChecked();
}

void MainTab::getLock_fs(){
     datagram.clear();
     datagram[0]=GET_DACLOCK_FS;
     WRITEDATAGRAM
}

void MainTab::slot_sendMute(bool state){
    datagram.clear();
    datagram[0]= MUTE_ALL;
    datagram[1]=state;
    WRITEDATAGRAM
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
    slot_sendMute(state);
}

void MainTab::slot_volumesliderChanged(int value){
    spinbox_volume->blockSignals(true);
    spinbox_volume->setValue(value);
    spinbox_volume->blockSignals(false);
    datagram.clear();
    datagram[0]= MASTERVOLUME_CHANGED;
    datagram[1]=(uchar) -2*value;
    WRITEDATAGRAM
    }

void MainTab::slot_volumeSpinboxChanged(int value){
    slider_MasterVolume->blockSignals(true);
    slider_MasterVolume->setValue(value);
    slider_MasterVolume->blockSignals(false);
    datagram.clear();
    datagram[0]= MASTERVOLUME_CHANGED;
    datagram[1]=(uchar) -2*value;
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


void MainTab::slot_inputselectorChanged(int index){
    datagram.clear();
    datagram[0] = INPUT_SOURCE;
    datagram[1] = (uchar) index;
    WRITEDATAGRAM
    if(index==2)
            setFormat(PCM);
}

void MainTab::sendProgram(){
    datagram[0]=(PROGRAM_CHANGED);
    datagram[1]=(char) getProgram();
    WRITEDATAGRAM
}

void MainTab::slot_programChanged(double program){
    //QTimer tmr;
    //tmr.setInterval(50);
    setMode(PRESET);
    datagram.clear();
    datagram[0]=(PROGRAM_CHANGED);
    datagram[1]=((char) round(program));
    WRITEDATAGRAM
   // tmr.start();
   // while( tmr.remainingTime()>0);
   // tmr.stop();
    }

void MainTab::setMode(int new_mode){
    mode=new_mode;
    if(mode==USER)
        program_Groupbox->setTitle("Current Mode: User defined");
    else
        program_Groupbox->setTitle("Current Mode: Preset");
}

int MainTab::getMode(){
    return mode;
}
