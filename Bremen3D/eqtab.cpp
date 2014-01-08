#include "eqtab.h"


// *******************************************************************************************



EQTab::EQTab(QWidget *parent , Network *udp) :
    QWidget(parent)
{


    channelTabs = new QTabWidget(this);
    layout = new QGridLayout(this);
    plot = new QCustomPlot(this);
    activeChannel=0;

    init_freqz(FMIN , FS/2);

    //Create channel tabs
    for(int i=0 ; i<CHANNELS ;i++){
        channel[i] = new EQChannel(this , i , plot ,udp );
        channelTabs->addTab(channel[i] , (QString("%1").arg(i)) );
        connect(channelTabs , SIGNAL(currentChanged(int)) , this , SLOT(slot_updatePlot(int)));
    }
    connect(channelTabs , SIGNAL(currentChanged(int)) , this , SLOT(slot_updatePlot(int)) );
    channel[activeChannel]->enableGraph();

    channelTabs->setToolTip(tr("Channel Selector"));
    channelTabs->setTabPosition(QTabWidget::West);
    layout->addWidget(channelTabs,0,0);
    layout->setContentsMargins(3,3,3,3);

    //   linked fc
    knob_linkedFc = new Knob(this,logScale);
    knob_linkedFc-> setTitle("Fc [Hz]");
    knob_linkedFc-> setKnobColor("rgb(255, 127, 127)");
    knob_linkedFc->setRange(10,20000,100);
    knob_linkedFc->setDecimals(0);
    knob_linkedFc->setSingleStep(1);
    knob_linkedFc->setValue(DEFAULT_FC);
    connect(knob_linkedFc , SIGNAL(valueChanged(double)) , this , SLOT(slot_linkedFcChanged(double)) );

    layout_linkedFc = new QVBoxLayout;
    layout_linkedFc->addWidget(knob_linkedFc);

    box_linkedFc = new QGroupBox;
    box_linkedFc->setLayout(layout_linkedFc);
    box_linkedFc->setTitle(tr("Linked fc"));
    box_linkedFc->setMaximumHeight(130);
     box_linkedFc->setMaximumWidth(80);
    layout->addWidget(box_linkedFc,1,0);

    plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot->xAxis->setAutoSubTicks(false);
    plot->xAxis->setSubTickCount(9);
    plot->xAxis->setLabel("Frequeny [Hz]");

    plot->yAxis->setLabel("Gain [dB]");
    plot->xAxis->setRange(FMIN , FS/2);
    plot->yAxis->setRange(-20, 10);

    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setAutoSubTicks(false);
    plot->yAxis->setTickStep(5);
    plot->yAxis->setSubTickCount(4);
    //plot->legend->setUserData();

    plot->replot();
    layout->addWidget(plot,0,1,2,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    setLayout(layout);
}

void EQTab::slot_linkedFcChanged(double value){
    for(int ch=0 ; ch<CHANNELS ;ch++)
        for(int sec=0 ; sec<SECTIONS ; sec++)
            if(channel[ch]->eqSection[sec]->link->isChecked())
                channel[ch]->eqSection[sec]->knob_fc->setValue(value);
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
     plot->graph(slaveCh)->setVisible(!state);
     plot->replot();

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
