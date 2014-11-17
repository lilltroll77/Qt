#include "eqtab.h"


// *******************************************************************************************



EQTab::EQTab(QWidget *parent , Network *udp) :
    QWidget(parent)
{

    main_tab=this->parent()->parent()->findChild<MainTab*>("MainTab");


    butterworthDialog =new QDialog(this);
    butterworthDialog->setMinimumWidth(480);
    butterworthDialog->setMinimumHeight(250);
    butterworthDialog->setWindowTitle("Q-value lookup table");
    //butterworthDialog->setWhatsThis("test");
    QVBoxLayout* butterworthLayout = new QVBoxLayout(this);
    butterworthTable = new QTableWidget(6 , 5 , this);
    QStringList horz;
    horz<<"Filter type"<<"Section 1"<<"Section 2"<<"Section 3"<<"Phase @ fc";
    butterworthTable->setHorizontalHeaderLabels(horz);
    int row,col;
    for(col=1 ; col<=4 ; col++)
        butterworthTable->setColumnWidth(col,80);


    const char first[]="1:st order";
    const char firstT[]="Select 1:st order filter";
    const char butt1[]="Butt-1";
    const char butt1T[]="First order Butterworth filter";
    const char butt3[]="Butt-3";
    const char butt3T[]="Third order Butterworth filter";
    const char butt5[]="Butt-5";
    const char butt5T[]="Fifth order Butterworth filter";
    const char lr2[]="LR-2";
    const char lr2T[]="Second order Linkwitz–Riley filter";
    const char lr4[]="LR-4";
    const char lr4T[]="Fourth order Linkwitz–Riley filter";
    const char lr6[]="LR-6";
    const char lr6T[]="Sixth order Linkwitz–Riley filter";
    const char invert[]="The polarity of the subwoofer channels should be selected to 'inverted' in DAC tab";
    float Q2=0.5;
    float Q3=1;
    float Q4[2]={1/sqrt(2),1/sqrt(2)};
    float Q5[2]={1/1.6180,1/0.6180};
    float Q6[3]={0.5 , 1 , 1};

    QBrush brush_lightred(QColor(255,192,192) , Qt::SolidPattern );

    for(row=0 ; row < ROWS ; row++)
        for(col=0 ; col < COLUMNS ; col++){
            butterworthTableItem[row][col] = new QTableWidgetItem(QTableWidgetItem::Type);
            butterworthTableItem[row][col]->setFlags(Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
            butterworthTableItem[row][col]->setTextAlignment(Qt::AlignCenter);
            butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
        }
row=0;
    col=0;
    butterworthTableItem[row][col]->setText(butt1);
    butterworthTableItem[row][col]->setToolTip(butt1T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);

    col=1;
    butterworthTableItem[row][col]->setText(first);
    butterworthTableItem[row][col]->setToolTip(firstT);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=90°"));
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);


row=1;
    col=0;
    butterworthTableItem[row][col]->setText(lr2);
    butterworthTableItem[row][col]->setToolTip(lr2T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=1;
    butterworthTableItem[row][col]->setText(QString("Q=%1").arg(Q2));
    butterworthTableItem[row][col]->setToolTip( QString("Select 2:nd order filter with Q=%1").arg(Q2));
    butterworthTableItem[row][col]->setTextAlignment(Qt::AlignCenter);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=180°"));
    butterworthTableItem[row][col]->setBackground(brush_lightred);
    butterworthTableItem[row][col]->setToolTip(invert);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);

row=2;
    col=0;
    butterworthTableItem[row][col]->setText(butt3);
    butterworthTableItem[row][col]->setToolTip(butt3T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=1;
    butterworthTableItem[row][col]->setText(first);
    butterworthTableItem[row][col]->setToolTip(firstT);
        butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=2;
    butterworthTableItem[row][col]->setText(QString("Q=%1").arg(Q3));
    butterworthTableItem[row][col]->setToolTip( QString("Select 2:nd order filter with Q=%1").arg(Q3));
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=-90°"));
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);

row=3;
    col=0;
    butterworthTableItem[row][col]->setText(tr(lr4));
    butterworthTableItem[row][col]->setToolTip(lr4T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    for(col=1 ; col<=2 ; col++){
        butterworthTableItem[row][col]->setText(QString("Q=%1").arg(Q4[col-1],0, 'f' ,3 ));
        butterworthTableItem[row][col]->setToolTip( QString("Select 2:nd order filter with Q=%1").arg(Q4[col-1],0, 'f' ,3 ));
        butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    }
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=0°"));
     butterworthTable->setItem(row, col, butterworthTableItem[row][col]);

row=4;
    col=0;
    butterworthTableItem[row][col]->setText(tr(butt5));
    butterworthTableItem[row][col]->setToolTip(butt5T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    col=1;
    butterworthTableItem[row][col]->setText(first);
    butterworthTableItem[row][col]->setToolTip(firstT);
    butterworthTableItem[row][col]->setTextAlignment(Qt::AlignCenter);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    for(col=2 ; col<=3 ; col++){
        butterworthTableItem[row][col]->setText(QString("Q=%1").arg(Q5[col-2],0 , 'f' ,3 ));
        butterworthTableItem[row][col]->setToolTip( QString("Select 2:nd order filter with Q=%1").arg(Q5[col-2],0 , 'f' ,3 ));
        butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    }
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=90°"));
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);


row=5;
    col=0;
    butterworthTableItem[row][col]->setText(tr(lr6));
    butterworthTableItem[row][col]->setToolTip(lr6T);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    for(col=1 ; col<=3 ; col++){
        butterworthTableItem[row][col]->setText(QString("Q=%1").arg(Q6[col-1],0 , 'f' ,3 ));
        butterworthTableItem[row][col]->setToolTip( QString("Select 2:nd order filter with Q=%1").arg(Q6[col-1],0 , 'f' ,3 ));
        butterworthTable->setItem(row, col, butterworthTableItem[row][col]);
    }
    col=4;
    butterworthTableItem[row][col]->setText(QString("φ=180°"));
    butterworthTableItem[row][col]->setBackground(brush_lightred);
    butterworthTableItem[row][col]->setToolTip(invert);
    butterworthTable->setItem(row, col, butterworthTableItem[row][col]);

    butterworthLayout->addWidget(butterworthTable);
    butterworthDialog->setLayout(butterworthLayout);


    UDP_Socket = udp->UDP_Socket;
    IP_XMOS = udp->IP_XMOS;
    port_XMOS = udp->port_XMOS;

    channelTabs = new QTabWidget(this);
    layout = new QGridLayout(this);
    plotMag = new QCustomPlot(this);
    activeChannel=0;

    calc_freqz(FMIN , FMAX , fs);

    //   linked fc
    knob_linkedFc = new Knob(this,logScale);
    knob_linkedFc-> setTitle("Fc [Hz]");
    knob_linkedFc-> setKnobColor("rgb(255, 127, 127)");
    knob_linkedFc->setRange(10,20000,100);
    knob_linkedFc->setDecimals(0);
    knob_linkedFc->setSingleStep(1);
    knob_linkedFc->setValue(DEFAULT_FC);
    knob_linkedFc->setObjectName("knob_linkedFc");
    connect(knob_linkedFc , SIGNAL(valueChanged(double)) , this , SLOT(slot_linkedFcChanged(double)) );


    knob_PreGain = new Knob(this, linScale);
    knob_PreGain -> setTitle("Gain [dB]");
    knob_PreGain -> setKnobColor("rgb(127, 127, 255)");
    knob_PreGain -> setRange(-20,0,200);
    knob_PreGain -> setDecimals(1);
    knob_PreGain -> setSingleStep(0.1);
    knob_PreGain -> setValue(DEFAULT_PREGAIN);
    connect(knob_PreGain , SIGNAL(valueChanged(double)) , this , SLOT(slot_PreGainChanged(double)) );



    //Create channel tabs
    for(int i=0 ; i<CHANNELS ;i++){
        channel[i] = new EQChannel(this , i , plotMag ,udp ,knob_linkedFc , &fs);
        channelTabs->addTab(channel[i] , (QString("%1").arg(i)) );
        connect(channelTabs , SIGNAL(currentChanged(int)) , this , SLOT(slot_updatePlot(int)));
    }
    connect(channelTabs , SIGNAL(currentChanged(int)) , this , SLOT(slot_updatePlot(int)) );
    channel[activeChannel]->enableGraph();

    channelTabs->setToolTip(tr("Channel Selector"));
    channelTabs->setTabPosition(QTabWidget::West);
    //channelTabs->setVisible(false);
    channelTabs->setMaximumWidth(SECTIONS*(SETMAXIMUMWIDTH+10)+140);
    channelTabs->setFixedHeight(450);
    layout->addWidget(channelTabs,0,0,1,5);
    layout->setContentsMargins(2,2,2,2);

    layout_linkedFc = new QVBoxLayout;
    layout_linkedFc->addWidget(knob_linkedFc);

    layout_PreGain = new QVBoxLayout;
    layout_PreGain->addWidget(knob_PreGain);

    box_linkedFc = new QGroupBox;
    box_linkedFc->setLayout(layout_linkedFc);
    box_linkedFc->setTitle(tr("Linked fc"));
    box_linkedFc->setMaximumHeight(130);
    box_linkedFc->setMaximumWidth(SETMAXIMUMWIDTH);
    layout->addWidget(box_linkedFc,1,2);

    box_PreGain = new QGroupBox;
    box_PreGain->setLayout(layout_PreGain);
    box_PreGain->setTitle(tr("Pre Gain"));
    box_PreGain->setMaximumHeight(130);
    box_PreGain->setMaximumWidth(SETMAXIMUMWIDTH);
    box_PreGain->setToolTip(tr("Sets the pre filter gain\nThis is needed when using filters with Q-values>0.5 to prevent clipping"));
    layout->addWidget(box_PreGain,1,1);

    button_Qlookup = new QPushButton("Q-Lookup"  ,this);
    button_Qlookup->setMaximumWidth(80);
    button_Qlookup->setToolTip("Lookup table for filter Q-values");
    connect(button_Qlookup , SIGNAL(clicked()) ,this , SLOT(slot_LookupTable()));
    layout->addWidget(button_Qlookup,1,3);

    plotMag->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plotMag->xAxis->setAutoSubTicks(false);
    plotMag->xAxis->setSubTickCount(9);
    plotMag->xAxis->setLabel("Frequeny [Hz]");

    plotMag->yAxis->setLabel("Gain [dB]");
    //plotMag->yAxis2->setVisible(true);
    //plotMag->yAxis2->setLabel("Phase [deg]");
    //plotMag->yAxis2->setRange(-180,180);

    plotMag->xAxis->setRange(FMIN , FS/2);
    plotMag->yAxis->setRange(-20, 10);

    plotMag->yAxis->setAutoTickStep(false);
    plotMag->yAxis->setAutoSubTicks(false);
    plotMag->yAxis->setTickStep(5);
    plotMag->yAxis->setSubTickCount(4);
    //plot->legend->setUserData();

    plotMag->setMinimumWidth(320);
    plotMag->replot();

    layout->addWidget(plotMag,0,5,2,1);
    for(int col=0; col<5 ; col++)
        layout->setColumnStretch(col,1);
    layout->setColumnStretch(5,2);
    //layout->setColumnStretch(3,1);
    setLayout(layout);
}

void EQTab::setPreGain(double pregain, bool blocked){
    if(blocked)
        knob_PreGain->blockSignals(true);
    knob_PreGain->setValue(pregain);
    if(blocked)
        knob_PreGain->blockSignals(false);
    }

double EQTab::getPreGain(){
    return knob_PreGain->Value();
}

void EQTab::slot_LookupTable(){
    butterworthDialog->show();
}

void EQTab::slot_sendPreGain(){
    float gain =(float) knob_PreGain->Value();
    const char* ptr = (const char*) &gain;
    datagram.clear();
    datagram[0] = PREGAIN_CHANGED;
    datagram.insert(4 , ptr , sizeof(float));
    WRITEDATAGRAM
}

void EQTab::updateLinkedFc(bool blocked){
    double fc=knob_linkedFc->Value();
    for(int ch=0 ; ch<CHANNELS ;ch++){
        for(int sec=0 ; sec<SECTIONS ; sec++)
            if(channel[ch]->eqSection[sec]->getLinked()){
                channel[ch]->eqSection[sec]->setFc(fc , false);
                //Recalc transfer function, but wait with replot
                channel[ch]->eqSection[sec]->updateSettingsAndPlot(false , fs);
        }
   channel[ch]->recalc_graph();
    }
}

void EQTab::setLinkedFc(double fc, bool blocked){
    main_tab->setMode(USER);
    if(blocked)
        knob_linkedFc->blockSignals(true);
    knob_linkedFc->setValue(fc);
    updateLinkedFc(blocked);
    if(blocked)
        knob_linkedFc->blockSignals(false);
    }

double EQTab::getLinkedFc(){
    return knob_linkedFc->Value();
}

void EQTab::slot_PreGainChanged(double gain){
    main_tab->setMode(USER);
    float gain_f = (float) gain;
    const char* ptr = (const char*) &gain_f;
    datagram.clear();
    datagram[0] = PREGAIN_CHANGED;
    datagram.insert(4 , ptr , sizeof(float));
    WRITEDATAGRAM
}

void EQTab::slot_linkedFcChanged(double fc){
    main_tab->setMode(USER);
    updateLinkedFc(true);


    /// Fix so the update of the plot only happens once

}

void EQTab::slot_updatePlot(int newChannel){
    channel[activeChannel]->disableGraph(); //Disable old channel graph
    channel[newChannel]->enableGraph();     //Enable new channel graph
    activeChannel=newChannel;

}
void EQTab::slot_linkchannel(int val){
    //channel*CHANNELS + ch
    int masterCh = val/CHANNELS;
    int slaveCh = val % CHANNELS;
    bool state;
    QFont font;
    state = channel[masterCh]->linkChannel[slaveCh]->isChecked();
     plotMag->graph(slaveCh)->setVisible(!state);
     plotMag->replot();

    for(int ch=0 ; ch< CHANNELS ; ch++){
        channel[slaveCh]->linkChannel[ch]->setVisible(!state);

        if(ch !=masterCh & ch!= slaveCh)
            channel[ch]->linkChannel[slaveCh]->setDisabled(state);
    }

    channel[slaveCh]->scrollArea->setVisible(!state);
    if(state){
        channel[slaveCh]->label_link->setText(QString("This channel is linked from channel %1").arg(masterCh));
        channel[slaveCh]->label_link->setAlignment(Qt::AlignTop | Qt::AlignVCenter );
        font.setPointSize(12);
        channel[slaveCh]->label_link->setFont(font);
        /// Fix the graph
    }else{
        channel[slaveCh]->label_link->setText(tr("Link to Ch: "));
        channel[slaveCh]->label_link->setAlignment(Qt::AlignLeft | Qt::AlignVCenter );
        font.setPointSize(8);
        channel[slaveCh]->label_link->setFont(font);
    }

   //channel[slaveCh]->linkChannel[masterCh]->setChecked(true);

}

void EQTab::fsChanged(int newfs){
    fs=newfs;
    calc_freqz(FMIN , fs/2 , fs);
    plotMag->xAxis->setRange(FMIN , fs/2);
     //recalc everything regarding graph
     for(int ch=0 ; ch<CHANNELS ;ch++){
         for(int sec=0 ; sec<SECTIONS ; sec++)
            channel[ch]->eqSection[sec]->updateSettingsAndPlot(false , fs);

        channel[ch]->recalc_graph();
     }
}


// **************************************************************

/*




 //SIGNAL
     connect(channelSelector , SIGNAL(currentIndexChanged(int)) , this , SLOT(updateEQchannel(int)) );
     connect(knob_delay , SIGNAL(valueChanged(double)) , this , SLOT(slot_send_Delay(double)) );
     setLayout(layout);
}

void EQChannel::updateEQchannel(int new_channel){
    knob_delay->blockSignals(true);
    knob_delay->setValue(delay[new_channel]); // update  knob to new value without telling XMOS
    knob_delay->blockSignals(false);

    for(int section=0 ; section<8 ;section++){
         eqSection[section]->groupBox->blockSignals(true);
         bool active =eqSettings.active[new_channel][section];
         eqSection[section]->groupBox->setChecked(active);
         eqSection[section]->groupBox->blockSignals(false);

         eqSection[section]->filterType->blockSignals(true);
         int type = (int) eqSettings.filtertype[new_channel][section];
         eqSection[section]->filterType->setCurrentIndex(type);
         eqSection[section]->filterType->blockSignals(false);

         bool state = eqSettings.link[new_channel][section];
         eqSection[section]->link->setChecked( state );

         eqSection[section]->knob_fc->blockSignals(true);
         double fc = eqSettings.fc[new_channel][section] ;
         eqSection[section]->knob_fc->setValue( fc );
         eqSection[section]->knob_fc->blockSignals(false);

         eqSection[section]->knob_Q->blockSignals(true);
         double Q = eqSettings.Q[new_channel][section];
         eqSection[section]->knob_Q->setValue(Q);
         eqSection[section]->knob_Q->blockSignals(false);

         eqSection[section]->knob_gain->blockSignals(true);
         double gain = eqSettings.gain[new_channel][section] ;
         eqSection[section]->knob_gain->setValue(gain);
         eqSection[section]->knob_gain->blockSignals(false);

         eqSection[section]->setChannelID(new_channel);
    }
    current_channel=new_channel;
}

int EQChannel::getCurrentChannel(){
    return current_channel;
}

void EQChannel::slot_send_Delay(double new_delay){
    delay[current_channel]=new_delay;
    datagram.clear();
    datagram.append( QString("DELAY CHANGED on Channel %1 to %2 ms").arg(current_channel).arg(new_delay));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *IP,  XMOS_PORT);
}

void EQChannel::setIP(QHostAddress *newIP){
    IP = newIP;
    for(int section=0 ; section<8 ;section++)
        eqSection[section]->setIP(IP);
}

void EQChannel::setPort(quint16 *new_port){
    port = new_port;
    for(int section=0 ; section<8 ;section++)
        eqSection[section]->setPort(port);
}
void EQChannel::setUDPsocket(QUdpSocket *new_udpSocket){
    udpSocket = new_udpSocket;
     for(int section=0 ; section<8 ;section++)
        eqSection[section]->setUDPsocket(udpSocket);
}

void EQChannel::setStatusBar(QStatusBar *new_statusbar){
    statusbar = new_statusbar;
}

*/
